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

    ////////////////////////////////////////////////////////////////
    // Setters.
    ////////////////////////////////////////////////////////////////

    void setWindowTransform(math::float4 lower, math::float4 upper);

private:
    math::mat4x4f windowTransform;
};

class WidgetTransformNode final : public sol::ForwardPushConstantNode, public floah::ITransformNode
{
public:
    WidgetTransformNode(sol::ForwardMaterial& material, math::float4 o) :
        ForwardPushConstantNode(material), offset(std::move(o))
    {
        // Enable first push constant.
        enablePushConstant(0);
    }

    [[nodiscard]] const void* getData(const size_t index) const override
    {
        assert(index == 0);

        return &offset;
    }

    ////////////////////////////////////////////////////////////////
    // Getters.
    ////////////////////////////////////////////////////////////////

    [[nodiscard]] Node& getAsNode() override { return *this; }

    [[nodiscard]] math::float4 getOffset() const noexcept { return offset; }

    ////////////////////////////////////////////////////////////////
    // Setters.
    ////////////////////////////////////////////////////////////////

    void setOffset(math::float3 o) noexcept override { offset = std::move(o); }

private:
    math::float4 offset;
};