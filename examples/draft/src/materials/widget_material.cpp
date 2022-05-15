#include "draft/materials/widget_material.h"

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "sol/material/forward/forward_material_manager.h"
#include "sol/texture/texture_manager.h"

WidgetMaterial::WidgetMaterial(sol::VulkanShaderModuleSharedPtr vertexModule,
                               sol::VulkanShaderModuleSharedPtr fragmentModule) :
    ForwardMaterial(std::move(vertexModule), std::move(fragmentModule))
{
    // TODO: We need to add this dummy because SOL crashes when you don't have any uniforms. That needs fixing, obvs.
    auto& ub0                 = layout.addUniformBuffer();
    ub0.name                  = "Dummy";
    ub0.set                   = 0;
    ub0.binding               = 0;
    ub0.size                  = sizeof(float);
    ub0.count                 = 1;
    ub0.stages                = VK_SHADER_STAGE_VERTEX_BIT;
    ub0.usage.updateDetection = sol::MaterialLayout::UpdateDetection::Manual;
    ub0.sharing.method        = sol::ForwardMaterialLayout::SharingMethod::None;
    ub0.sharing.count         = 1;

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
}

WidgetMaterial::~WidgetMaterial() noexcept = default;

WidgetMaterialInstance::~WidgetMaterialInstance() noexcept = default;

uint32_t WidgetMaterialInstance::getSetIndex() const { return 0; }

bool WidgetMaterialInstance::isUniformBufferStale(size_t binding) const { return false; }

const void* WidgetMaterialInstance::getUniformBufferData(size_t binding) const { return &dummy; }

sol::Texture2D* WidgetMaterialInstance::getTextureData(size_t binding) const { return nullptr; }
