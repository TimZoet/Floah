#pragma once

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "sol/core/fwd.h"
#include "sol/material/forward/forward_material.h"
#include "sol/material/forward/forward_material_instance.h"

class WidgetMaterial final : public sol::ForwardMaterial
{
public:
    WidgetMaterial() = default;

    WidgetMaterial(sol::VulkanShaderModuleSharedPtr vertexModule, sol::VulkanShaderModuleSharedPtr fragmentModule);

    WidgetMaterial(const WidgetMaterial&) = delete;

    WidgetMaterial(WidgetMaterial&&) = delete;

    ~WidgetMaterial() noexcept override;

    WidgetMaterial& operator=(const WidgetMaterial&) = delete;

    WidgetMaterial& operator=(WidgetMaterial&&) = delete;
};

class WidgetMaterialInstance final : public sol::ForwardMaterialInstance
{
public:
    WidgetMaterialInstance() = default;

    WidgetMaterialInstance(const WidgetMaterialInstance&) = delete;

    WidgetMaterialInstance(WidgetMaterialInstance&&) = delete;

    ~WidgetMaterialInstance() noexcept override;

    WidgetMaterialInstance& operator=(const WidgetMaterialInstance&) = delete;

    WidgetMaterialInstance& operator=(WidgetMaterialInstance&&) = delete;

    ////////////////////////////////////////////////////////////////
    // Getters.
    ////////////////////////////////////////////////////////////////

    [[nodiscard]] uint32_t getSetIndex() const override;

    [[nodiscard]] bool isUniformBufferStale(size_t binding) const override;

    [[nodiscard]] const void* getUniformBufferData(size_t binding) const override;

    [[nodiscard]] sol::Texture2D* getTextureData(size_t binding) const override;

private:
    float dummy = 0;
};
