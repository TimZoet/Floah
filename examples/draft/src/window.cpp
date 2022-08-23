#include "draft/window.h"

////////////////////////////////////////////////////////////////
// Standard includes.
////////////////////////////////////////////////////////////////

#include <fstream>

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "common/enum_classes.h"
#include "floah-layout/elements/horizontal_flow.h"
#include "floah-widget/node_masks.h"
#include "floah-widget/widgets/checkbox.h"
#include "floah-widget/widgets/dropdown.h"
#include "floah-widget/widgets/radio_button.h"
#include "luna/window.h"
#include "luna/command/command_queue.h"
#include "luna/command/material/forward/update_forward_material_manager_command.h"
#include "luna/command/other/custom_command.h"
#include "luna/command/other/fence_command.h"
#include "luna/command/other/submit_command.h"
#include "luna/command/present/acquire_command.h"
#include "luna/command/present/present_command.h"
#include "luna/command/render/forward/forward_render_command.h"
#include "luna/command/render/forward/forward_traverse_command.h"
#include "luna/shader_cache/shader_cache.h"
#include "sol/core/vulkan_attachment.h"
#include "sol/core/vulkan_command_buffer_list.h"
#include "sol/core/vulkan_device.h"
#include "sol/core/vulkan_fence.h"
#include "sol/core/vulkan_frame_buffer.h"
#include "sol/core/vulkan_instance.h"
#include "sol/core/vulkan_physical_device.h"
#include "sol/core/vulkan_render_pass.h"
#include "sol/core/vulkan_render_pass_layout.h"
#include "sol/core/vulkan_semaphore.h"
#include "sol/core/vulkan_subpass.h"
#include "sol/core/vulkan_surface.h"
#include "sol/core/vulkan_swapchain.h"
#include "sol/material/forward/forward_material_manager.h"
#include "sol/memory/memory_manager.h"
#include "sol/mesh/mesh_manager.h"
#include "sol/render/common/render_settings.h"
#include "sol/render/forward/forward_render_data.h"
#include "sol/render/forward/forward_renderer.h"
#include "sol/render/forward/forward_traverser.h"
#include "sol/scenegraph/scenegraph.h"
#include "sol/scenegraph/drawable/mesh_node.h"
#include "sol/scenegraph/forward/forward_material_node.h"
#include "sol/texture/image2d.h"
#include "sol/texture/texture_manager.h"
#include "sol/texture/texture2d.h"

////////////////////////////////////////////////////////////////
// Current target includes.
////////////////////////////////////////////////////////////////

#include "draft/application.h"

namespace
{
    void cursorPositionCallback(GLFWwindow* window, const double xpos, const double ypos)
    {
        floah::InputContext& context = *static_cast<floah::InputContext*>(glfwGetWindowUserPointer(window));
        context.setCursor(math::int2(math::floor(xpos), math::floor(ypos)));
    }

    void mouseButtonCallback(GLFWwindow* window, const int button, const int action, int mods)
    {
        floah::InputContext& context = *static_cast<floah::InputContext*>(glfwGetWindowUserPointer(window));

        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            context.setMouseButton(floah::InputContext::MouseButton::Left,
                                   action == GLFW_PRESS ? floah::InputContext::MouseAction::Press :
                                                          floah::InputContext::MouseAction::Release,
                                   static_cast<floah::InputContext::MouseModifiers>(mods));
        }
        else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
            context.setMouseButton(floah::InputContext::MouseButton::Middle,
                                   action == GLFW_PRESS ? floah::InputContext::MouseAction::Press :
                                                          floah::InputContext::MouseAction::Release,
                                   static_cast<floah::InputContext::MouseModifiers>(mods));
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            context.setMouseButton(floah::InputContext::MouseButton::Right,
                                   action == GLFW_PRESS ? floah::InputContext::MouseAction::Press :
                                                          floah::InputContext::MouseAction::Release,
                                   static_cast<floah::InputContext::MouseModifiers>(mods));
        }
    }

    void windowFocusCallback(GLFWwindow* window, const int focused)
    {
        floah::InputContext& context = *static_cast<floah::InputContext*>(glfwGetWindowUserPointer(window));
        context.setFocus(focused ? true : false);
        std::cout << "Focus " << focused << std::endl;
    }

    void cursorEnterCallback(GLFWwindow* window, const int entered)
    {
        floah::InputContext& context = *static_cast<floah::InputContext*>(glfwGetWindowUserPointer(window));
        context.setEnter(entered ? true : false);
        std::cout << "Enter " << entered << std::endl;
    }
}  // namespace

////////////////////////////////////////////////////////////////
// Constructors.
////////////////////////////////////////////////////////////////

Window::Window(Application& app, std::string windowName, const int32_t w, const int32_t h) :
    application(&app), name(std::move(windowName)), width(w), height(h)
{
}

Window::~Window() noexcept = default;

////////////////////////////////////////////////////////////////
// Init.
////////////////////////////////////////////////////////////////

void Window::initialize()
{
    window       = std::make_unique<sol::Window>(std::array{width, height}, "ForwardRenderer");
    inputContext = std::make_unique<floah::InputContext>();
    scenegraph   = std::make_unique<sol::Scenegraph>();
    createDataSources();
    createSurface();
    createSwapchain();
    createRenderPass();
    createRenderer();
    createSyncObjects();
    createFramebuffers();
    createCommandBuffers();
    createMaterials();
    createSwapchainRenderData();
    createPanel();

    glfwSetWindowUserPointer(window->get(), inputContext.get());
    glfwSetCursorPosCallback(window->get(), cursorPositionCallback);
    glfwSetCursorEnterCallback(window->get(), cursorEnterCallback);
    glfwSetMouseButtonCallback(window->get(), mouseButtonCallback);
    glfwSetWindowFocusCallback(window->get(), windowFocusCallback);
}

void Window::createSurface()
{
    sol::VulkanSurface::Settings surfaceSettings;
    surfaceSettings.instance = application->device->getPhysicalDevice().getInstance();
    surfaceSettings.func     = [this](const sol::VulkanInstance& i, VkSurfaceKHR* s) {
        return glfwCreateWindowSurface(i.get(), window->get(), nullptr, s);
    };
    surface = sol::VulkanSurface::create(surfaceSettings);
}

void Window::createSwapchain()
{
    sol::VulkanSwapchain::Settings swapchainSettings;
    swapchainSettings.surface        = surface;
    swapchainSettings.physicalDevice = application->device->getPhysicalDevice();
    swapchainSettings.device         = *application->device;
    swapchainSettings.extent         = {.width = static_cast<uint32_t>(width), .height = static_cast<uint32_t>(height)};
    application->device->getPhysicalDevice().recreateSwapchainSupportDetails(surface.get());
    swapchain = sol::VulkanSwapchain::create(swapchainSettings);
}

void Window::createRenderPass()
{
    {
        sol::VulkanRenderPassLayout renderPassLayout;
        auto&                       colorAttachment = renderPassLayout.createAttachment();
        colorAttachment.setFormat(VK_FORMAT_R32G32B32A32_SFLOAT);
        colorAttachment.setSamples(VK_SAMPLE_COUNT_1_BIT);
        colorAttachment.setLoadOp(VK_ATTACHMENT_LOAD_OP_CLEAR);
        colorAttachment.setStoreOp(VK_ATTACHMENT_STORE_OP_STORE);
        colorAttachment.setStencilLoadOp(VK_ATTACHMENT_LOAD_OP_DONT_CARE);
        colorAttachment.setStencilStoreOp(VK_ATTACHMENT_STORE_OP_DONT_CARE);
        colorAttachment.setInitialLayout(VK_IMAGE_LAYOUT_UNDEFINED);
        colorAttachment.setFinalLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        auto& depthAttachment = renderPassLayout.createAttachment();
        depthAttachment.setFormat(VK_FORMAT_D32_SFLOAT);
        depthAttachment.setSamples(VK_SAMPLE_COUNT_1_BIT);
        depthAttachment.setLoadOp(VK_ATTACHMENT_LOAD_OP_CLEAR);
        depthAttachment.setStoreOp(VK_ATTACHMENT_STORE_OP_DONT_CARE);
        depthAttachment.setStencilLoadOp(VK_ATTACHMENT_LOAD_OP_DONT_CARE);
        depthAttachment.setStencilStoreOp(VK_ATTACHMENT_STORE_OP_DONT_CARE);
        depthAttachment.setInitialLayout(VK_IMAGE_LAYOUT_UNDEFINED);
        depthAttachment.setFinalLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        auto& subpass = renderPassLayout.createSubpass();
        subpass.setPipelineBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS);
        subpass.addColorAttachment(colorAttachment, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        subpass.setDepthStencilAttachment(depthAttachment, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        subpass.addExternalDependency(
          true,
          VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
          0,
          VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
          VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
          0);

        renderPassLayout.finalize();

        sol::VulkanRenderPass::Settings renderPassSettings;
        renderPassSettings.device = *application->device;
        renderPassSettings.layout = &renderPassLayout;
        renderPass                = sol::VulkanRenderPass::create(renderPassSettings);
    }

    {
        sol::VulkanRenderPassLayout renderPassLayout;

        auto& colorAttachment = renderPassLayout.createAttachment();
        colorAttachment.setFormat(swapchain->getSurfaceFormat().format);
        colorAttachment.setSamples(VK_SAMPLE_COUNT_1_BIT);
        colorAttachment.setLoadOp(VK_ATTACHMENT_LOAD_OP_CLEAR);
        colorAttachment.setStoreOp(VK_ATTACHMENT_STORE_OP_STORE);
        colorAttachment.setStencilLoadOp(VK_ATTACHMENT_LOAD_OP_DONT_CARE);
        colorAttachment.setStencilStoreOp(VK_ATTACHMENT_STORE_OP_DONT_CARE);
        colorAttachment.setInitialLayout(VK_IMAGE_LAYOUT_UNDEFINED);
        colorAttachment.setFinalLayout(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

        auto& subpass = renderPassLayout.createSubpass();
        subpass.setPipelineBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS);
        subpass.addColorAttachment(colorAttachment, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

        subpass.addExternalDependency(true,
                                      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                      0,
                                      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                                      0);

        renderPassLayout.finalize();

        sol::VulkanRenderPass::Settings renderPassSettings;
        renderPassSettings.device = *application->device;
        renderPassSettings.layout = &renderPassLayout;
        swapchainRenderPass       = sol::VulkanRenderPass::create(renderPassSettings);
    }
}

void Window::createRenderer()
{
    renderData          = std::make_unique<sol::ForwardRenderData>();
    swapchainRenderData = std::make_unique<sol::ForwardRenderData>();
    traverser           = std::make_unique<sol::ForwardTraverser>();
    renderer            = std::make_unique<sol::ForwardRenderer>();

    traverser->setTypeMaskFunction([](uint64_t mask) {
        if (mask & static_cast<uint64_t>(floah::NodeMasks::Disabled))
            return sol::ITraverser::TraversalAction::Terminate;
        return sol::ITraverser::TraversalAction::Continue;
    });
    renderer->getRenderSettings()->setCullMode(sol::RenderSettings::CullMode::None);
}

void Window::createSyncObjects()
{
    sol::VulkanFence::Settings fenceSettings;
    fenceSettings.device   = *application->device;
    fenceSettings.signaled = true;
    sol::VulkanSemaphore::Settings semSettings;
    semSettings.device = *application->device;

    for (size_t i = 0; i < Application::maxFrames; i++)
    {
        sync.inUse.emplace_back(sol::VulkanFence::create(fenceSettings));
        sync.swapchainInUse.emplace_back(sol::VulkanFence::create(fenceSettings));
        sync.renderFinished.emplace_back(sol::VulkanSemaphore::create(semSettings));
        sync.swapchainRenderFinished.emplace_back(sol::VulkanSemaphore::create(semSettings));
        sync.imageAvailable.emplace_back(sol::VulkanSemaphore::create(semSettings));
    }
}

void Window::createFramebuffers()
{
    for (size_t i = 0; i < Application::maxFrames; i++)
    {
        auto& colorImage =
          application->textureManager->createImage2D(VK_FORMAT_R32G32B32A32_SFLOAT,
                                                     {swapchain->getExtent().width, swapchain->getExtent().height},
                                                     VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
        auto& colorTexture = application->textureManager->createTexture2D(colorImage);
        swapchainTextures.emplace_back(&colorTexture);

        auto& depthImage = application->textureManager->createImage2D(
          VK_FORMAT_D32_SFLOAT, colorImage.getSize(), VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
        depthImage.setTargetLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        depthImage.setAspectFlags(VK_IMAGE_ASPECT_DEPTH_BIT);
        auto& depthTexture = application->textureManager->createTexture2D(depthImage);

        sol::VulkanFramebuffer::Settings framebufferSettings;
        framebufferSettings.renderPass = renderPass;
        framebufferSettings.attachments.emplace_back(colorTexture.getImageView());
        framebufferSettings.attachments.emplace_back(depthTexture.getImageView());
        framebufferSettings.width  = swapchain->getExtent().width;
        framebufferSettings.height = swapchain->getExtent().height;
        framebuffers.emplace_back(sol::VulkanFramebuffer::create(framebufferSettings));
    }

    for (size_t i = 0; i < swapchain->getImageCount(); i++)
    {
        sol::VulkanFramebuffer::Settings framebufferSettings;
        framebufferSettings.renderPass = swapchainRenderPass;
        framebufferSettings.attachments.emplace_back(swapchain->getImageViews()[i].get());
        framebufferSettings.width  = swapchain->getExtent().width;
        framebufferSettings.height = swapchain->getExtent().height;
        swapchainFramebuffers.emplace_back(sol::VulkanFramebuffer::create(framebufferSettings));
    }
}

void Window::createCommandBuffers()
{
    sol::VulkanCommandBufferList::Settings commandBufferSettings;
    commandBufferSettings.commandPool = *application->commandPool;

    commandBufferSettings.size = Application::maxFrames;
    commandBuffers             = sol::VulkanCommandBufferList::create(commandBufferSettings);

    commandBufferSettings.size = swapchain->getImageCount();
    swapchainCommandBuffers    = sol::VulkanCommandBufferList::create(commandBufferSettings);
}

void Window::createMaterials()
{
    materialManager = std::make_unique<sol::ForwardMaterialManager>(*application->memoryManager);
    materialManager->setDataSetCount(swapchain->getImageCount());

    widgetMaterial = &materialManager->addMaterial(std::make_unique<WidgetMaterial>(
      application->materials.widgetVertShader, application->materials.widgetFragShader));
    widgetMaterial->setMeshLayout(*application->meshLayout);
    widgetMaterialInstance =
      &materialManager->addMaterialInstance(*widgetMaterial, std::make_unique<WidgetMaterialInstance>());
    widgetMaterialInstance->setWindowTransform(math::float4(0, 0, -1000, 0), math::float4(width, height, 1000, 0));

    textMaterial = &materialManager->addMaterial(
      std::make_unique<TextMaterial>(application->materials.textVertShader, application->materials.textFragShader));
    textMaterial->setMeshLayout(*application->meshLayout);
    textMaterialInstance0 = &materialManager->addMaterialInstance(
      *textMaterial, std::make_unique<TextMaterialInstance>(*application->materials.fontmap->getTexture()));
    textMaterialInstance1 =
      &materialManager->addMaterialInstance(*textMaterial, std::make_unique<TextMaterialInstance>());
    textMaterialInstance1->setWindowTransform(math::float4(0, 0, -1000, 0), math::float4(width, height, 1000, 0));

    auto vertShader =
      application->shaderCache->getModule("vertex/display")->createVulkanShaderModuleShared(*application->device);
    auto fragShader =
      application->shaderCache->getModule("fragment/display")->createVulkanShaderModuleShared(*application->device);

    swapchainMaterial = &materialManager->addMaterial(std::make_unique<DisplayMaterial>(vertShader, fragShader));
    swapchainMaterial->setMeshLayout(*application->displayMeshLayout);
    swapchainMaterialInstance = &materialManager->addMaterialInstance(
      *swapchainMaterial, std::make_unique<DisplayMaterialInstance>(*swapchainTextures.front()));
}

void Window::createCommands(sol::ICommand&                            pollCmd,
                            sol::UpdateForwardMaterialManagerCommand& updateMtlManagerCmd,
                            sol::UpdateMeshManagerCommand&            updateMeshManagerCmd)
{
    auto& commandQueue = pollCmd.getCommandQueue();

    auto& regenerateCommand = commandQueue.createCommand<sol::CustomCommand>();
    regenerateCommand.setFunction([&] {
        panel->generateWidgetLayouts();
        panel->generateGeometry(*application->meshManager, *application->materials.fontmap);
        panel->generateScenegraph(*scenegraphGenerator);
    });
    regenerateCommand.addDependency(pollCmd);

    auto& traverseCommand = commandQueue.createCommand<sol::ForwardTraverseCommand>();
    traverseCommand.setName("Traverse");
    traverseCommand.setRenderData(*renderData);
    traverseCommand.setTraverser(*traverser);
    traverseCommand.setScenegraph(*scenegraph);
    traverseCommand.addDependency(regenerateCommand);

    auto& awaitFenceCommand = commandQueue.createCommand<sol::FenceCommand>();
    awaitFenceCommand.setName("Await Fence");
    awaitFenceCommand.setAction(sol::FenceCommand::Action::Wait | sol::FenceCommand::Action::Reset);
    for (auto& fence : sync.inUse) awaitFenceCommand.addFence(*fence);
    awaitFenceCommand.setFenceIndexPtr(&application->frameIdx);
    awaitFenceCommand.addDependency(traverseCommand);

    auto& updateMaterialCommand = commandQueue.createCommand<sol::CustomCommand>();
    updateMaterialCommand.setName("Update Material");
    updateMaterialCommand.addDependency(awaitFenceCommand);
    updateMaterialCommand.setFunction(
      [&] { swapchainMaterialInstance->setTexture(*swapchainTextures[application->frameIdx]); });

    auto& updateWindowMtlManagerCmd = commandQueue.createCommand<sol::UpdateForwardMaterialManagerCommand>();
    updateWindowMtlManagerCmd.setName("Update Forward Material Manager");
    updateWindowMtlManagerCmd.setMaterialManager(*materialManager);
    updateWindowMtlManagerCmd.setImageIndexPtr(&application->frameIdx);
    updateWindowMtlManagerCmd.addDependency(updateMaterialCommand);

    auto& renderCommand = commandQueue.createCommand<sol::ForwardRenderCommand>();
    renderCommand.setName("Render");
    renderCommand.setRenderer(*renderer);
    renderCommand.setRenderData(*renderData);
    renderCommand.setRenderPass(*renderPass);
    renderCommand.setCommandBufferList(*commandBuffers);
    for (const auto& fb : framebuffers) renderCommand.addFramebuffer(*fb);
    renderCommand.setImageIndexPtr(&application->frameIdx);
    renderCommand.addDependency(updateWindowMtlManagerCmd);

    updateMtlManagerCmd.addDependency(awaitFenceCommand);

    auto& swapchainAwaitFenceCommand = commandQueue.createCommand<sol::FenceCommand>();
    swapchainAwaitFenceCommand.setName("Await Fence (Swapchain)");
    swapchainAwaitFenceCommand.setAction(sol::FenceCommand::Action::Wait | sol::FenceCommand::Action::Reset);
    for (auto& fence : sync.swapchainInUse) swapchainAwaitFenceCommand.addFence(*fence);
    swapchainAwaitFenceCommand.setFenceIndexPtr(&application->frameIdx);

    auto& submitCommand = commandQueue.createCommand<sol::SubmitCommand>();
    submitCommand.setName("Submit");
    submitCommand.setQueue(application->memoryManager->getGraphicsQueue());
    submitCommand.setCommandBufferList(*commandBuffers);
    submitCommand.setCommandBufferIndexPtr(&application->frameIdx);
    for (auto& sem : sync.renderFinished) submitCommand.addSignalSemaphore(*sem);
    submitCommand.setSignalSemaphoreIndexPtr(&application->frameIdx);
    for (auto& fence : sync.inUse) submitCommand.addSignalFence(*fence);
    submitCommand.setSignalFenceIndexPtr(&application->frameIdx);
    // Wait for command buffer recording to complete.
    submitCommand.addDependency(renderCommand);
    // Wait for all material data to be updated.
    submitCommand.addDependency(updateMtlManagerCmd);
    submitCommand.addDependency(updateMeshManagerCmd);
    // Wait for swapchain submit to complete, because it uses the texture this submit renders to.
    submitCommand.addDependency(swapchainAwaitFenceCommand);

    auto& acquireCommand = commandQueue.createCommand<sol::AcquireCommand>();
    acquireCommand.setName("Acquire");
    acquireCommand.setSwapchain(*swapchain);
    for (auto& sem : sync.imageAvailable) acquireCommand.addSignalSemaphore(*sem);
    acquireCommand.setSignalSemaphoreIndexPtr(&application->frameIdx);
    acquireCommand.setImageIndexPtr(&imageIdx);
    acquireCommand.addDependency(swapchainAwaitFenceCommand);

    auto& swapchainRenderCommand = commandQueue.createCommand<sol::ForwardRenderCommand>();
    swapchainRenderCommand.setName("Render (Swapchain)");
    swapchainRenderCommand.setRenderer(*renderer);
    swapchainRenderCommand.setRenderData(*swapchainRenderData);
    swapchainRenderCommand.setRenderPass(*swapchainRenderPass);
    swapchainRenderCommand.setCommandBufferList(*swapchainCommandBuffers);
    for (const auto& fb : swapchainFramebuffers) swapchainRenderCommand.addFramebuffer(*fb);
    swapchainRenderCommand.setImageIndexPtr(&imageIdx);
    swapchainRenderCommand.addDependency(acquireCommand);

    auto& swapchainSubmitCommand = commandQueue.createCommand<sol::SubmitCommand>();
    swapchainSubmitCommand.setName("Submit (Display)");
    swapchainSubmitCommand.setQueue(application->memoryManager->getGraphicsQueue());
    swapchainSubmitCommand.setCommandBufferList(*swapchainCommandBuffers);
    swapchainSubmitCommand.setCommandBufferIndexPtr(&imageIdx);
    for (size_t i = 0; i < Application::maxFrames; i++)
    {
        swapchainSubmitCommand.addWaitSemaphore(*sync.renderFinished[i], VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
        swapchainSubmitCommand.addWaitSemaphore(*sync.imageAvailable[i], VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
    }
    swapchainSubmitCommand.setWaitSemaphoreFunction(
      [this](const uint32_t i) { return i / 2 == application->frameIdx; });
    for (auto& sem : sync.swapchainRenderFinished) swapchainSubmitCommand.addSignalSemaphore(*sem);
    swapchainSubmitCommand.setSignalSemaphoreIndexPtr(&application->frameIdx);
    for (auto& fence : sync.swapchainInUse) swapchainSubmitCommand.addSignalFence(*fence);
    swapchainSubmitCommand.setSignalFenceIndexPtr(&application->frameIdx);
    swapchainSubmitCommand.addDependency(swapchainRenderCommand);
    // Wait for render to texture submit, because renderFinished semaphore needs to be submitted.
    swapchainSubmitCommand.addDependency(submitCommand);

    auto& presentCommand = commandQueue.createCommand<sol::PresentCommand>();
    presentCommand.setName("Present");
    presentCommand.setSwapchain(*swapchain);
    for (auto& sem : sync.swapchainRenderFinished) presentCommand.addWaitSemaphore(*sem);
    presentCommand.setWaitSemaphoreIndexPtr(&application->frameIdx);
    presentCommand.setImageIndexPtr(&imageIdx);
    presentCommand.addDependency(swapchainSubmitCommand);

    const auto recreate = [&](sol::VulkanSwapchain&) {
        framebuffers.clear();
        swapchainFramebuffers.clear();
        swapchainTextures.clear();
        // TODO: Destroy images and textures.
        createFramebuffers();
        // TODO: This is a bit of a hack (see also comment on getFrameBuffers method).
        renderCommand.getFramebuffers().clear();
        swapchainRenderCommand.getFramebuffers().clear();
        for (auto& f : framebuffers) renderCommand.getFramebuffers().push_back(f.get());
        for (auto& f : swapchainFramebuffers) swapchainRenderCommand.getFramebuffers().push_back(f.get());
    };

    acquireCommand.setRecreateFunction(recreate);
    presentCommand.setRecreateFunction(recreate);
}

void Window::createPanel()
{
    panelStylesheet = std::make_unique<floah::Stylesheet>();
    panelStylesheet->set(floah::Widget::material_text,
                         static_cast<sol::ForwardMaterialInstance*>(textMaterialInstance1));
    panelStylesheet->set(floah::Widget::material_widget,
                         static_cast<sol::ForwardMaterialInstance*>(widgetMaterialInstance));

    checkboxStylesheet = std::make_unique<floah::Stylesheet>();
    //panelStylesheet->set(floah::Checkbox::checkbox_box_size, floah::Size(1.0f, 0.5f));
    //checkboxStylesheet->set(floah::Checkbox::checkbox_box_size, floah::Size(1.0f, 0.25f));
    //panelStylesheet->set(floah::Checkbox::checkbox_box_height, floah::Length(1.0f));
    //checkboxStylesheet->set(floah::Checkbox::checkbox_box_height, floah::Length(0.5f));

    panel = std::make_unique<floah::Panel>(*inputContext);
    panel->setStylesheet(panelStylesheet.get());
    auto& topLayer = panel->createLayer("top", -1);

    auto& layout = panel->getLayout();
    layout.getSize().setWidth(floah::Length(width));
    layout.getSize().setHeight(floah::Length(height));

    auto& root = layout.setRoot(std::make_unique<floah::VerticalFlow>());
    root.getSize().setWidth(floah::Length(1.0f));
    root.getSize().setHeight(floah::Length(1.0f));
    auto& dropdownElem = root.append(std::make_unique<floah::LayoutElement>());
    dropdownElem.getSize().setWidth(floah::Length(1.0f));
    dropdownElem.getSize().setHeight(floah::Length(32));
    auto& checkbox0Elem = root.append(std::make_unique<floah::LayoutElement>());
    checkbox0Elem.getSize().setWidth(floah::Length(1.0f));
    checkbox0Elem.getSize().setHeight(floah::Length(32));
    auto& checkbox1Elem = root.append(std::make_unique<floah::LayoutElement>());
    checkbox1Elem.getSize().setWidth(floah::Length(1.0f));
    checkbox1Elem.getSize().setHeight(floah::Length(32));

    dataSources.stringList.list->appendString("abc");
    dataSources.stringList.list->appendString("def");
    dataSources.stringList.list->appendString("ghi");
    dataSources.stringList.list->appendString("jkl");
    auto& dropdown = panel->addWidget(std::make_unique<floah::Dropdown>(), topLayer);
    dropdown.setLabel("A Value");
    dropdown.setPanelLayoutElement(dropdownElem);
    dropdown.setStylesheet(checkboxStylesheet.get());
    dropdown.setItemsDataSource(dataSources.stringList.list.get());
    dropdown.setIndexDataSource(dataSources.stringList.index.get());

    auto& checkbox0 = panel->addWidget(std::make_unique<floah::RadioButton>(), topLayer);
    checkbox0.setLabel("Some Toggle");
    checkbox0.setPanelLayoutElement(checkbox0Elem);
    checkbox0.setStylesheet(checkboxStylesheet.get());
    checkbox0.setDataSource(dataSources.val0Source.get());

    auto& checkbox1 = panel->addWidget(std::make_unique<floah::RadioButton>(checkbox0), topLayer);
    checkbox1.setLabel("Another Toggle");
    checkbox1.setPanelLayoutElement(checkbox1Elem);
    checkbox1.setStylesheet(checkboxStylesheet.get());
    checkbox1.setDataSource(dataSources.val1Source.get());

    scenegraphGenerator =
      std::make_unique<ScenegraphGenerator>(*widgetMaterial, *textMaterialInstance0, scenegraph->getRootNode());

    panel->generatePanelLayout();
    panel->generateWidgetLayouts();
    panel->generateGeometry(*application->meshManager, *application->materials.fontmap);
    panel->generateScenegraph(*scenegraphGenerator);

    //application->meshManager->transferStagedCopies();

    dot::Graph graph;
    scenegraph->visualize(graph);
    std::ofstream f(std::string("scenegraph") + name + ".dot");
    graph.write(f);
}

void Window::createSwapchainRenderData()
{
    sol::Scenegraph graph;
    auto&           mtlNode =
      graph.getRootNode().addChild(std::make_unique<sol::ForwardMaterialNode>(*swapchainMaterialInstance));
    mtlNode.addChild(std::make_unique<sol::MeshNode>(*application->quadMesh));

    traverser->traverse(graph, *swapchainRenderData);
}

void Window::createDataSources()
{
    dataSources.val0Source = std::make_unique<floah::BoolDataSource>(dataSources.val0);
    dataSources.val1Source = std::make_unique<floah::BoolDataSource>(dataSources.val1);
}
