#pragma once

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "math/include_all.h"
#include "floah-viz/scenegraph/transform_node.h"
#include "sol/core/fwd.h"
#include "sol/material/forward/forward_material.h"
#include "sol/material/forward/forward_material_instance.h"
#include "sol/scenegraph/forward/forward_push_constant_node.h"

class WidgetTransformNode final : public sol::ForwardPushConstantNode, public floah::ITransformNode
{
public:
    WidgetTransformNode(sol::ForwardMaterial& material, math::float4 o) :
        ForwardPushConstantNode(material), offset(std::move(o))
    {
    }

    [[nodiscard]] std::pair<size_t, size_t> getRange() const noexcept override
    {
        return std::make_pair<size_t, size_t>(16, 16);
    }

    [[nodiscard]] VkShaderStageFlags getStageFlags() const noexcept override { return VK_SHADER_STAGE_VERTEX_BIT; }

    [[nodiscard]] const void* getData() const override { return &offset; }

    ////////////////////////////////////////////////////////////////
    // Getters.
    ////////////////////////////////////////////////////////////////

    [[nodiscard]] Node& getAsNode() override { return *this; }

    [[nodiscard]] math::float4 getOffset() const noexcept { return offset; }

    ////////////////////////////////////////////////////////////////
    // Setters.
    ////////////////////////////////////////////////////////////////

    void setOffset(math::float3 o) noexcept override { offset = std::move(o); }

    void setZ(const float z) override { offset.z = z; }

private:
    math::float4 offset;
};