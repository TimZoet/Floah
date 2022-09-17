#pragma once

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "floah-viz/scenegraph/scenegraph_generator.h"
#include "sol/material/fwd.h"

class ScenegraphGenerator : public floah::IScenegraphGenerator
{
public:
    ////////////////////////////////////////////////////////////////
    // Constructors.
    ////////////////////////////////////////////////////////////////

    ScenegraphGenerator();

    ScenegraphGenerator(sol::ForwardMaterial&         wdgtMaterial,
                        sol::ForwardMaterialInstance& txtMaterialInstance,
                        sol::Node&                    root);

    ScenegraphGenerator(const ScenegraphGenerator&);

    ScenegraphGenerator(ScenegraphGenerator&&) noexcept;

    ~ScenegraphGenerator() noexcept override;

    ScenegraphGenerator& operator=(const ScenegraphGenerator&);

    ScenegraphGenerator& operator=(ScenegraphGenerator&&) noexcept;

    ////////////////////////////////////////////////////////////////
    // Getters.
    ////////////////////////////////////////////////////////////////

    [[nodiscard]] sol::Node& createPanelNode(sol::Node* parent) override;

    [[nodiscard]] floah::ITransformNode& createPanelTransformNode(sol::Node& parent, math::float3 offset) override;

    [[nodiscard]] sol::Node& createWidgetNode(sol::Node* parent) override;

    [[nodiscard]] sol::Node& createTextMaterialNode(sol::Node&                    parent,
                                                    sol::ForwardMaterialInstance& mtlInstance) override;

    [[nodiscard]] floah::ITransformNode& createWidgetTransformNode(sol::Node& parent, math::float3 offset) override;

    [[nodiscard]] sol::NodePtr createMaterialNode(sol::ForwardMaterialInstance& mtlInstance) override;

private:
    ////////////////////////////////////////////////////////////////
    // Member variables.
    ////////////////////////////////////////////////////////////////

    sol::ForwardMaterial* widgetMaterial = nullptr;

    sol::ForwardMaterialInstance* textMaterialInstance = nullptr;

    sol::Node* rootNode = nullptr;
};
