#pragma once

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "math/include_all.h"
#include "sol/material/forward/forward_material_instance.h"
#include "sol/texture/texture2d.h"

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

    ////////////////////////////////////////////////////////////////
    // Setters.
    ////////////////////////////////////////////////////////////////

    void setFontmap(sol::Texture2D& fmap);

    void setWindowTransform(math::float4 lower, math::float4 upper);

private:
    math::mat4x4f   windowTransform;
    sol::Texture2D* fontmap = nullptr;
};
