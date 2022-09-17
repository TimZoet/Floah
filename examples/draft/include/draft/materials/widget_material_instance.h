#pragma once

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "math/include_all.h"
#include "sol/material/forward/forward_material_instance.h"

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
