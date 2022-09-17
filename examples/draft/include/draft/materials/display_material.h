#pragma once

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "sol/core/fwd.h"
#include "sol/material/forward/forward_material.h"

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
