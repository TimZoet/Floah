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
    auto& sampler   = layout.addSampler();
    sampler.name    = "Fontmap";
    sampler.set     = 0;
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
}

TextMaterial::~TextMaterial() noexcept = default;

TextMaterialInstance::TextMaterialInstance(sol::Texture2D& fmap) : fontmap(&fmap) {}

TextMaterialInstance::~TextMaterialInstance() noexcept = default;

uint32_t TextMaterialInstance::getSetIndex() const { return 0; }

bool TextMaterialInstance::isUniformBufferStale(size_t binding) const { return false; }

const void* TextMaterialInstance::getUniformBufferData(size_t binding) const { return nullptr; }

sol::Texture2D* TextMaterialInstance::getTextureData(size_t binding) const { return fontmap; }
