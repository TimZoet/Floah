#pragma once

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "sol/core/fwd.h"
#include "sol/material/forward/forward_material.h"

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
