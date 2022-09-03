#pragma once

////////////////////////////////////////////////////////////////
// Standard includes.
////////////////////////////////////////////////////////////////

#include <string>
#include <vector>

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "floah-data/bool_data_source.h"
#include "floah-put/input_context.h"
#include "floah-widget/panel.h"
#include "luna/fwd.h"
#include "luna/command/fwd.h"
#include "luna/command/material/forward/update_forward_material_manager_command.h"
#include "luna/command/mesh/update_mesh_manager_command.h"
#include "sol/core/fwd.h"
#include "sol/material/fwd.h"
#include "sol/render/forward/fwd.h"
#include "sol/scenegraph/fwd.h"

////////////////////////////////////////////////////////////////
// Current target includes.
////////////////////////////////////////////////////////////////

#include "draft/data_sources.h"
#include "draft/generators.h"
#include "draft/materials/display_material.h"
#include "draft/materials/text_material.h"
#include "draft/materials/widget_material.h"

class Application;

class Window
{
public:
    friend class Application;

    ////////////////////////////////////////////////////////////////
    // Constructors.
    ////////////////////////////////////////////////////////////////

    Window() = default;

    Window(Application& app, std::string windowName, int32_t w, int32_t h);

    Window(const Window&) = delete;

    Window(Window&&) noexcept = delete;

    ~Window() noexcept;

    Window& operator=(const Window&) = delete;

    Window& operator=(Window&&) noexcept = delete;

    ////////////////////////////////////////////////////////////////
    // Init.
    ////////////////////////////////////////////////////////////////

    void initialize();

private:
    void createSurface();

    void createSwapchain();

    void createRenderPass();

    void createRenderer();

    void createSyncObjects();

    void createFramebuffers();

    void createCommandBuffers();

    void createMaterials();

    void createCommands(sol::ICommand&                            pollCmd,
                        sol::UpdateForwardMaterialManagerCommand& updateMtlManagerCmd,
                        sol::UpdateMeshManagerCommand&            transferMeshManagerCmd);

    void createPanel();

    void createSwapchainRenderData();

    void createDataSources();

    ////////////////////////////////////////////////////////////////
    // Member variables.
    ////////////////////////////////////////////////////////////////

    Application* application = nullptr;

    std::string name;

    int32_t width = 0;

    int32_t height = 0;

    sol::WindowPtr window;

    std::unique_ptr<floah::InputContext> inputContext;

    sol::VulkanSurfacePtr surface;

    sol::VulkanSwapchainPtr swapchain;

    std::vector<sol::VulkanFramebufferPtr> framebuffers;

    std::vector<sol::VulkanFramebufferPtr> swapchainFramebuffers;

    std::vector<sol::Texture2D*> swapchainTextures;

    sol::VulkanRenderPassPtr renderPass;

    sol::VulkanRenderPassPtr swapchainRenderPass;

    sol::ForwardRenderDataPtr renderData;

    sol::ForwardRenderDataPtr swapchainRenderData;

    sol::ForwardTraverserPtr traverser;

    sol::ForwardRendererPtr renderer;

    sol::ScenegraphPtr scenegraph;

    sol::VulkanCommandBufferListPtr commandBuffers;

    sol::VulkanCommandBufferListPtr swapchainCommandBuffers;

    struct
    {
        std::vector<sol::VulkanFencePtr> inUse;

        std::vector<sol::VulkanFencePtr> swapchainInUse;

        /**
         * \brief Semaphores indicating the render to texture has finished.
         */
        std::vector<sol::VulkanSemaphorePtr> renderFinished;

        /**
         * \brief Semaphores indicating the render to the swapchain has finished.
         */
        std::vector<sol::VulkanSemaphorePtr> swapchainRenderFinished;

        std::vector<sol::VulkanSemaphorePtr> imageAvailable;
    } sync;

    /**
     * \brief Index of last acquired image.
     */
    uint32_t imageIdx = 0;

    floah::PanelPtr panel;

    sol::ForwardMaterialManagerPtr materialManager;

    sol::ForwardMaterial* widgetMaterial = nullptr;

    WidgetMaterialInstance* widgetMaterialInstance = nullptr;

    sol::ForwardMaterial* textMaterial = nullptr;

    TextMaterialInstance* textMaterialInstance0 = nullptr;

    TextMaterialInstance* textMaterialInstance1 = nullptr;

    sol::ForwardMaterial* swapchainMaterial = nullptr;

    DisplayMaterialInstance* swapchainMaterialInstance = nullptr;

    std::unique_ptr<ScenegraphGenerator> scenegraphGenerator;

    floah::StylesheetPtr panelStylesheet;

    floah::StylesheetPtr checkboxStylesheet;

    struct
    {
        bool                                    val0 = false;
        bool                                    val1 = true;
        std::unique_ptr<floah::IBoolDataSource> val0Source;
        std::unique_ptr<floah::IBoolDataSource> val1Source;
        IndexedStringList                       stringList;
    } dataSources;
};
