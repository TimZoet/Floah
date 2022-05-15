#pragma once

////////////////////////////////////////////////////////////////
// Standard includes.
////////////////////////////////////////////////////////////////

#include <vector>

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "floah-viz/font_map.h"
#include "luna/fwd.h"
#include "luna/command/fwd.h"
#include "luna/shader_cache/fwd.h"
#include "sol/core/fwd.h"
#include "sol/material/fwd.h"
#include "sol/memory/fwd.h"
#include "sol/mesh/fwd.h"
#include "sol/texture/fwd.h"

////////////////////////////////////////////////////////////////
// Current target includes.
////////////////////////////////////////////////////////////////

#include "draft/window.h"

class Application
{
public:
    friend class Window;
    static constexpr uint32_t maxFrames = 2;

    ////////////////////////////////////////////////////////////////
    // Constructors.
    ////////////////////////////////////////////////////////////////

    Application();

    Application(const Application&) = delete;

    Application(Application&&) noexcept = delete;

    ~Application() noexcept;

    Application& operator=(const Application&) = delete;

    Application& operator=(Application&&) noexcept = delete;

    ////////////////////////////////////////////////////////////////
    // ...
    ////////////////////////////////////////////////////////////////

    Window& createWindow(std::string name, int32_t width, int32_t height);

    ////////////////////////////////////////////////////////////////
    // Init.
    ////////////////////////////////////////////////////////////////

    void initialize();

    void createCommandQueue();

    void run();

private:
    void createInstance();

    void createPhysicalDevice();

    void createDevice();

    void createManagers();

    void createCommandPool();

    void createMeshLayouts();

    void createShaderCache();

    void createMaterials();

    ////////////////////////////////////////////////////////////////
    // Member variables.
    ////////////////////////////////////////////////////////////////

    sol::VulkanInstancePtr instance;

    sol::VulkanPhysicalDevicePtr physicalDevice;

    sol::VulkanDevicePtr device;

    sol::VulkanCommandPoolPtr commandPool;

    sol::MemoryManagerPtr memoryManager;

    sol::TextureManagerPtr textureManager;

    sol::ForwardMaterialManagerPtr materialManager;

    sol::MeshManagerPtr meshManager;

    sol::MeshLayout* meshLayout = nullptr;

    sol::MeshLayout* displayMeshLayout = nullptr;

    sol::ShaderCachePtr shaderCache;

    struct
    {
        std::unique_ptr<floah::FontMap> fontmap;
        sol::ForwardMaterial*           widget          = nullptr;
        sol::ForwardMaterialInstance*   widgetInstance  = nullptr;
        sol::ForwardMaterial*           text            = nullptr;
        sol::ForwardMaterialInstance*   textInstance    = nullptr;
    } materials;

    sol::CommandQueuePtr commandQueue;

    std::vector<std::unique_ptr<Window>> windows;

    uint32_t frameIdx = 0;

    sol::IMesh* quadMesh = nullptr;
};
