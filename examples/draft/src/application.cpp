#include "draft/application.h"

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "floah-viz/vertex.h"
#include "luna/command/command_queue.h"
#include "luna/command/material/forward/update_forward_material_manager_command.h"
#include "luna/command/mesh/update_mesh_manager_command.h"
#include "luna/command/other/custom_command.h"
#include "luna/shader_cache/shader_cache.h"
#include "luna/window.h"
#include "sol/core/vulkan_command_pool.h"
#include "sol/core/vulkan_device.h"
#include "sol/core/vulkan_instance.h"
#include "sol/core/vulkan_physical_device.h"
#include "sol/core/vulkan_queue.h"
#include "sol/core/vulkan_shader_module.h"
#include "sol/core/vulkan_surface.h"
#include "sol/material/forward/forward_material_manager.h"
#include "sol/memory/memory_manager.h"
#include "sol/mesh/indexed_mesh.h"
#include "sol/mesh/mesh_description.h"
#include "sol/mesh/mesh_manager.h"
#include "sol/texture/texture_manager.h"

////////////////////////////////////////////////////////////////
// Current target includes.
////////////////////////////////////////////////////////////////

#include "draft/materials/text_material.h"
#include "draft/materials/widget_material.h"

namespace
{
    std::vector              validationLayers = {std::string("VK_LAYER_KHRONOS_validation")};
    std::vector<std::string> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
}  // namespace

////////////////////////////////////////////////////////////////
// Constructors.
////////////////////////////////////////////////////////////////

Application::Application() = default;

Application::~Application() noexcept { vkDeviceWaitIdle(device->get()); }

////////////////////////////////////////////////////////////////
// ...
////////////////////////////////////////////////////////////////

Window& Application::createWindow(std::string name, const int32_t width, const int32_t height)
{
    auto& window = *windows.emplace_back(std::make_unique<Window>(*this, std::move(name), width, height));
    window.initialize();
    return window;
}

////////////////////////////////////////////////////////////////
// Init.
////////////////////////////////////////////////////////////////

void Application::initialize()
{
    createInstance();
    createPhysicalDevice();
    createDevice();
    createManagers();
    createCommandPool();
    createMeshLayouts();
    createShaderCache();
    createMaterials();
}

void Application::createCommandQueue()
{
    commandQueue = std::make_unique<sol::CommandQueue>();

    auto& pollCommand = commandQueue->createCommand<sol::CustomCommand>();
    pollCommand.setName("Poll");
    pollCommand.setFunction([&] {
        glfwPollEvents();
        for (const auto& window : windows) window->inputContext->frame();
    });


    auto& updateMtlManagerCmd = commandQueue->createCommand<sol::UpdateForwardMaterialManagerCommand>();
    updateMtlManagerCmd.setName("Update Forward Material Manager");
    updateMtlManagerCmd.setMaterialManager(*materialManager);
    updateMtlManagerCmd.setImageIndexPtr(&frameIdx);

    for (const auto& window : windows) window->createCommands(pollCommand, updateMtlManagerCmd);

    commandQueue->finalize();
}

void Application::run()
{
    while (true)  //!glfwWindowShouldClose(window)
    {
        commandQueue->start();

        auto cmd = commandQueue->getNextCommand();

        while (cmd)
        {
            (*cmd)();
            cmd = commandQueue->getNextCommand();
        }

        commandQueue->end();

        frameIdx = (frameIdx + 1) % maxFrames;
    }
}

void Application::createInstance()
{
    // TODO: Need to create a window here, otherwise sol::Window::getRequiredExtensions() returns an empty list. Has to do with static initialization, I guess. Not exactly sure why, though.
    auto                          window = std::make_unique<sol::Window>(std::array{32, 32}, "");
    sol::VulkanInstance::Settings instanceSettings;
    instanceSettings.applicationName = "ForwardRenderer";
    instanceSettings.extensions      = sol::Window::getRequiredExtensions();
    instanceSettings.extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    instanceSettings.enableDebugging  = true;
    instanceSettings.enableValidation = true;
    instanceSettings.validationLayers = validationLayers;
    instance                          = sol::VulkanInstance::create(instanceSettings);
}

void Application::createPhysicalDevice()
{
    // Create temporary window and surface needed for physical device creation.
    auto                         window = std::make_unique<sol::Window>(std::array{32, 32}, "");
    sol::VulkanSurface::Settings surfaceSettings;
    surfaceSettings.instance = instance;
    surfaceSettings.func     = [&window](const sol::VulkanInstance& i, VkSurfaceKHR* s) {
        return glfwCreateWindowSurface(i.get(), window->get(), nullptr, s);
    };
    auto surface = sol::VulkanSurface::create(surfaceSettings);

    sol::VulkanPhysicalDevice::Settings physicalDeviceSettings;
    physicalDeviceSettings.instance = instance;
    physicalDeviceSettings.surface  = surface;
    physicalDeviceSettings.extensions.assign(deviceExtensions.begin(), deviceExtensions.end());
    physicalDeviceSettings.propertyFilter = [](const VkPhysicalDeviceProperties& props) {
        return props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
    };
    physicalDeviceSettings.queueFamilyFilter = [](const std::vector<sol::VulkanQueueFamily>& queues) {
        bool hasGraphics          = false;
        bool hasPresent           = false;
        bool hasDedicatedTransfer = false;

        for (const auto& q : queues)
        {
            if (q.supportsGraphics()) hasGraphics = true;
            if (q.supportsPresent()) hasPresent = true;
            if (q.supportsDedicatedTransfer()) hasDedicatedTransfer = true;
        }

        return hasGraphics && hasPresent && hasDedicatedTransfer;
    };
    physicalDevice = sol::VulkanPhysicalDevice::create(physicalDeviceSettings);
}

void Application::createDevice()
{
    sol::VulkanDevice::Settings deviceSettings;
    deviceSettings.physicalDevice = physicalDevice;
    deviceSettings.extensions     = physicalDevice->getSettings().extensions;
    deviceSettings.queues.resize(physicalDevice->getQueueFamilies().size());
    std::ranges::fill(deviceSettings.queues.begin(), deviceSettings.queues.end(), 1);
    deviceSettings.threadSafeQueues = true;
    device                          = sol::VulkanDevice::create(deviceSettings);
}

void Application::createManagers()
{
    memoryManager = std::make_unique<sol::MemoryManager>(*device);
    for (auto& queue : device->getQueues())
    {
        if (queue->getFamily().supportsCompute()) memoryManager->setComputeQueue(*queue);
        if (queue->getFamily().supportsGraphics()) memoryManager->setGraphicsQueue(*queue);
        if (queue->getFamily().supportsDedicatedTransfer()) memoryManager->setTransferQueue(*queue);
    }

    textureManager = std::make_unique<sol::TextureManager>(*memoryManager);

    materialManager = std::make_unique<sol::ForwardMaterialManager>(*memoryManager);
    materialManager->setDataSetCount(maxFrames);

    meshManager = std::make_unique<sol::MeshManager>(*memoryManager);
}

void Application::createCommandPool()
{
    sol::VulkanCommandPool::Settings commandPoolSettings;
    commandPoolSettings.device           = *device;
    commandPoolSettings.queueFamilyIndex = memoryManager->getGraphicsQueue().getFamily().getIndex();
    commandPoolSettings.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPool                          = sol::VulkanCommandPool::create(commandPoolSettings);
}

void Application::createMeshLayouts()
{
    meshLayout = &meshManager->createMeshLayout("gui");
    meshLayout->addBinding("vertex", 0, sizeof(floah::Vertex), VK_VERTEX_INPUT_RATE_VERTEX);
    meshLayout->addAttribute("pos", 0, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(floah::Vertex, position));
    meshLayout->addAttribute("col", 1, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(floah::Vertex, color));
    meshLayout->addAttribute("uv", 2, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(floah::Vertex, uv));
    meshLayout->finalize();

    displayMeshLayout = &meshManager->createMeshLayout("display");
    displayMeshLayout->addBinding("vertex", 0, 16, VK_VERTEX_INPUT_RATE_VERTEX);
    displayMeshLayout->addAttribute("pos", 0, 0, VK_FORMAT_R32G32_SFLOAT, 0);
    displayMeshLayout->addAttribute("uv", 1, 0, VK_FORMAT_R32G32_SFLOAT, 8);
    displayMeshLayout->finalize();

    auto quadDescription = meshManager->createMeshDescription();
    quadDescription->addVertexBuffer(16, 4);
    quadDescription->addIndexBuffer(2, 6);

    const std::array quadVerts = {math::float2(-1, -1),
                                  math::float2(0, 0),
                                  math::float2(1, -1),
                                  math::float2(1, 0),
                                  math::float2(1, 1),
                                  math::float2(1, 1),
                                  math::float2(-1, 1),
                                  math::float2(0, 1)};

    const std::array<int16_t, 6> quadIndices = {0, 1, 2, 2, 3, 0};

    quadDescription->setVertexData(0, 0, 4, quadVerts.data());
    quadDescription->setIndexData(0, 6, quadIndices.data());

    quadMesh = &meshManager->createIndexedMesh(std::move(quadDescription));
}

void Application::createShaderCache()
{
    auto [cache, created] = sol::ShaderCache::openOrCreate("shadercache.db");

    const auto importAndCompile = [&cache](const std::string&                        shaderFile,
                                           std::string                               identifier,
                                           const sol::VulkanShaderModule::ShaderType shaderType) {
        const auto source = cache->importSource(
          shaderFile, std::move(identifier), shaderType, sol::ShaderCache::OverwriteAction::Different);

        const auto modules = source->getModules();
        if (modules.empty()) { static_cast<void>(source->compile(source->getIdentifier(), false)); }
        else
        {
            for (auto& m : modules)
            {
                if (m->getSourceChecksum() != source->getChecksum()) m->recompile();
            }
        }
    };
    // TODO: Get rid of hardcoded paths.
    importAndCompile(R"(C:\\Users\\timzo\\dev\\projects\\Floah\\source\\examples\\draft\\shaders\\widget.vert)",
                     "vertex/widget",
                     sol::VulkanShaderModule::ShaderType::Vertex);

    importAndCompile(R"(C:\\Users\\timzo\\dev\\projects\\Floah\\source\\examples\\draft\\shaders\\widget.frag)",
                     "fragment/widget",
                     sol::VulkanShaderModule::ShaderType::Fragment);

    importAndCompile(R"(C:\\Users\\timzo\\dev\\projects\\Floah\\source\\examples\\draft\\shaders\\text.vert)",
                     "vertex/text",
                     sol::VulkanShaderModule::ShaderType::Vertex);

    importAndCompile(R"(C:\\Users\\timzo\\dev\\projects\\Floah\\source\\examples\\draft\\shaders\\text.frag)",
                     "fragment/text",
                     sol::VulkanShaderModule::ShaderType::Fragment);

    importAndCompile(R"(C:\\Users\\timzo\\dev\\projects\\Floah\\source\\examples\\draft\\shaders\\display.vert)",
                     "vertex/display",
                     sol::VulkanShaderModule::ShaderType::Vertex);

    importAndCompile(R"(C:\\Users\\timzo\\dev\\projects\\Floah\\source\\examples\\draft\\shaders\\display.frag)",
                     "fragment/display",
                     sol::VulkanShaderModule::ShaderType::Fragment);

    shaderCache = std::move(cache);
}

void Application::createMaterials()
{
    // TODO: Retrieve font from elsewhere. Ship with application, if license allows it?
    materials.fontmap =
      std::make_unique<floah::FontMap>(R"(C:\Users\timzo\AppData\Local\Microsoft\Windows\Fonts\FiraCode-Regular.ttf)",
                                       std::make_pair<uint32_t, uint32_t>(0x0001, 0x024F),
                                       math::uint2{0, 24});
    materials.fontmap->generateTexture(*textureManager);
    textureManager->transfer();

    auto widgetVertShader = shaderCache->getModule("vertex/widget")->createVulkanShaderModuleShared(*device);
    auto widgetFragShader = shaderCache->getModule("fragment/widget")->createVulkanShaderModuleShared(*device);
    auto textVertShader   = shaderCache->getModule("vertex/text")->createVulkanShaderModuleShared(*device);
    auto textFragShader   = shaderCache->getModule("fragment/text")->createVulkanShaderModuleShared(*device);

    materials.widget =
      &materialManager->addMaterial(std::make_unique<WidgetMaterial>(widgetVertShader, widgetFragShader));
    materials.widget->setMeshLayout(*meshLayout);
    materials.widgetInstance =
      &materialManager->addMaterialInstance(*materials.widget, std::make_unique<WidgetMaterialInstance>());

    materials.text = &materialManager->addMaterial(std::make_unique<TextMaterial>(textVertShader, textFragShader));
    materials.text->setMeshLayout(*meshLayout);
    materials.textInstance = &materialManager->addMaterialInstance(
      *materials.text, std::make_unique<TextMaterialInstance>(*materials.fontmap->getTexture()));
}
