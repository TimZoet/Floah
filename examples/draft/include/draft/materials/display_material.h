#pragma once

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "sol/core/fwd.h"
#include "sol/material/forward/forward_material.h"
#include "sol/material/forward/forward_material_instance.h"
#include "sol/texture/texture2d.h"

class DisplayMaterial final : public sol::ForwardMaterial
{
public:
    DisplayMaterial() = default;

    DisplayMaterial(sol::VulkanShaderModuleSharedPtr vertexModule, sol::VulkanShaderModuleSharedPtr fragmentModule);

    DisplayMaterial(const DisplayMaterial&) = delete;

    DisplayMaterial(DisplayMaterial&&) = delete;

    ~DisplayMaterial() noexcept override;

    DisplayMaterial& operator=(const DisplayMaterial&) = delete;

    DisplayMaterial& operator=(DisplayMaterial&&) = delete;
};

class DisplayMaterialInstance final : public sol::ForwardMaterialInstance
{
public:
    DisplayMaterialInstance() = default;

    explicit DisplayMaterialInstance(sol::Texture2D& tex);

    DisplayMaterialInstance(const DisplayMaterialInstance&) = delete;

    DisplayMaterialInstance(DisplayMaterialInstance&&) = delete;

    ~DisplayMaterialInstance() noexcept override;

    DisplayMaterialInstance& operator=(const DisplayMaterialInstance&) = delete;

    DisplayMaterialInstance& operator=(DisplayMaterialInstance&&) = delete;

    ////////////////////////////////////////////////////////////////
    // Getters.
    ////////////////////////////////////////////////////////////////

    [[nodiscard]] uint32_t getSetIndex() const override;

    [[nodiscard]] bool isUniformBufferStale(size_t binding) const override;

    [[nodiscard]] const void* getUniformBufferData(size_t binding) const override;

    [[nodiscard]] sol::Texture2D* getTextureData(size_t binding) const override;

    ////////////////////////////////////////////////////////////////
    // Setters.
    ////////////////////////////////////////////////////////////////

    void setTexture(sol::Texture2D& tex);

private:
    sol::Texture2D* texture = nullptr;
};
