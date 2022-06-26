#pragma once

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "floah-widget/generators/scenegraph_generator.h"
#include "sol/material/fwd.h"

class ScenegraphGenerator : public floah::IScenegraphGenerator
{
public:
    ////////////////////////////////////////////////////////////////
    // Constructors.
    ////////////////////////////////////////////////////////////////

    ScenegraphGenerator();

    ScenegraphGenerator(sol::ForwardMaterial& wdgtMaterial, sol::ForwardMaterial& txtMaterial, sol::Node& wdgtNode, sol::Node& txtNode);

    ScenegraphGenerator(const ScenegraphGenerator&);

    ScenegraphGenerator(ScenegraphGenerator&&) noexcept;

    ~ScenegraphGenerator() noexcept override;

    ScenegraphGenerator& operator=(const ScenegraphGenerator&);

    ScenegraphGenerator& operator=(ScenegraphGenerator&&) noexcept;

    ////////////////////////////////////////////////////////////////
    // Getters.
    ////////////////////////////////////////////////////////////////

    [[nodiscard]] sol::Node& createWidgetNode(math::float3 offset) override;

    [[nodiscard]] sol::Node& createTextNode(math::float3 offset) override;

private:
    ////////////////////////////////////////////////////////////////
    // Member variables.
    ////////////////////////////////////////////////////////////////

    sol::ForwardMaterial* widgetMaterial = nullptr;

    sol::ForwardMaterial* textMaterial = nullptr;

    /**
     * \brief Root node for widget nodes.
     */
    sol::Node* widgetNode = nullptr;

    /**
     * \brief Root node for text nodes.
     */
    sol::Node* textNode = nullptr;
};
