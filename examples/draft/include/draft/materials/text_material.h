#pragma once

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "sol/core/fwd.h"
#include "sol/material/forward/forward_material.h"
#include "sol/material/forward/forward_material_instance.h"
#include "sol/texture/texture2d.h"

class TextMaterial final : public sol::ForwardMaterial
{
public:
    TextMaterial() = default;

    TextMaterial(sol::VulkanShaderModuleSharedPtr vertexModule, sol::VulkanShaderModuleSharedPtr fragmentModule);

    TextMaterial(const TextMaterial&) = delete;

    TextMaterial(TextMaterial&&) = delete;

    ~TextMaterial() noexcept override;

    TextMaterial& operator=(const TextMaterial&) = delete;

    TextMaterial& operator=(TextMaterial&&) = delete;
};

class TextMaterialInstance final : public sol::ForwardMaterialInstance
{
public:
    TextMaterialInstance() = default;

    explicit TextMaterialInstance(sol::Texture2D& fmap);

    TextMaterialInstance(const TextMaterialInstance&) = delete;

    TextMaterialInstance(TextMaterialInstance&&) = delete;

    ~TextMaterialInstance() noexcept override;

    TextMaterialInstance& operator=(const TextMaterialInstance&) = delete;

    TextMaterialInstance& operator=(TextMaterialInstance&&) = delete;

    ////////////////////////////////////////////////////////////////
    // Getters.
    ////////////////////////////////////////////////////////////////

    [[nodiscard]] uint32_t getSetIndex() const override;

    [[nodiscard]] bool isUniformBufferStale(size_t binding) const override;

    [[nodiscard]] const void* getUniformBufferData(size_t binding) const override;

    [[nodiscard]] sol::Texture2D* getTextureData(size_t binding) const override;

private:
    sol::Texture2D* fontmap = nullptr;
};
