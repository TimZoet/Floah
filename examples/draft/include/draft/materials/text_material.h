#pragma once

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "sol/core/fwd.h"
#include "sol/material/forward/forward_material.h"

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
