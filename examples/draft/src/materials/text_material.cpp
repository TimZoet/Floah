#include "draft/materials/text_material.h"

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "sol/material/forward/forward_material_manager.h"
#include "sol/texture/texture_manager.h"

TextMaterial::TextMaterial(sol::VulkanShaderModuleSharedPtr vertexModule,
                           sol::VulkanShaderModuleSharedPtr fragmentModule) :
    ForwardMaterial(std::move(vertexModule), std::move(fragmentModule))
{
    auto& windowTransform                 = layout.addUniformBuffer();
    windowTransform.name                  = "WindowTransform";
    windowTransform.set                   = 0;
    windowTransform.binding               = 0;
    windowTransform.size                  = sizeof(float) * 8;
    windowTransform.count                 = 1;
    windowTransform.stages                = VK_SHADER_STAGE_VERTEX_BIT;
    windowTransform.usage.updateDetection = sol::MaterialLayout::UpdateDetection::Always;
    windowTransform.sharing.method        = sol::ForwardMaterialLayout::SharingMethod::None;
    windowTransform.sharing.count         = 1;

    auto& widgetTransform  = layout.addPushConstant();
    widgetTransform.name   = "WidgetTransform";
    widgetTransform.size   = sizeof(float) * 8;
    widgetTransform.offset = 0;
    widgetTransform.stages = VK_SHADER_STAGE_VERTEX_BIT;

    auto& sampler   = layout.addSampler();
    sampler.name    = "Fontmap";
    sampler.set     = 1;
    sampler.binding = 0;
    sampler.stages  = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkPipelineColorBlendAttachmentState colorBlending;
    colorBlending.blendEnable         = VK_TRUE;
    colorBlending.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlending.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlending.colorBlendOp        = VK_BLEND_OP_ADD;
    colorBlending.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlending.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlending.alphaBlendOp        = VK_BLEND_OP_ADD;
    colorBlending.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    layout.addColorBlending(colorBlending);

    layout.finalize();

    setLayer(1);
}

TextMaterial::~TextMaterial() noexcept = default;
