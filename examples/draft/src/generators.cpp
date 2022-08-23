#include "draft/generators.h"

////////////////////////////////////////////////////////////////
// Current target includes.
////////////////////////////////////////////////////////////////

#include "draft/materials/widget_material.h"

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "sol/scenegraph/forward/forward_material_node.h"

////////////////////////////////////////////////////////////////
// Constructors.
////////////////////////////////////////////////////////////////

ScenegraphGenerator::ScenegraphGenerator() = default;

ScenegraphGenerator::ScenegraphGenerator(sol::ForwardMaterial&         wdgtMaterial,
                                         sol::ForwardMaterialInstance& txtMaterialInstance,
                                         sol::Node&                    root) :
    widgetMaterial(&wdgtMaterial), textMaterialInstance(&txtMaterialInstance), rootNode(&root)
{
}

ScenegraphGenerator::ScenegraphGenerator(const ScenegraphGenerator&) = default;

ScenegraphGenerator::ScenegraphGenerator(ScenegraphGenerator&&) noexcept = default;

ScenegraphGenerator::~ScenegraphGenerator() noexcept = default;

ScenegraphGenerator& ScenegraphGenerator::operator=(const ScenegraphGenerator&) = default;

ScenegraphGenerator& ScenegraphGenerator::operator=(ScenegraphGenerator&&) noexcept = default;

////////////////////////////////////////////////////////////////
// Getters.
////////////////////////////////////////////////////////////////

sol::Node& ScenegraphGenerator::createWidgetNode() { return rootNode->addChild(std::make_unique<sol::Node>()); }

sol::Node& ScenegraphGenerator::createTextMaterialNode(sol::Node& parent, sol::ForwardMaterialInstance& mtlInstance)
{
    auto& node = parent.addChild(std::make_unique<sol::ForwardMaterialNode>(*textMaterialInstance));
    return node.addChild(std::make_unique<sol::ForwardMaterialNode>(mtlInstance));
}

floah::ITransformNode& ScenegraphGenerator::createTransformNode(sol::Node& parent, const math::float3 offset)
{
    return parent.addChild(std::make_unique<WidgetTransformNode>(*widgetMaterial, math::float4(offset, 1.0f)));
}

sol::NodePtr ScenegraphGenerator::createMaterialNode(sol::ForwardMaterialInstance& mtlInstance)
{
    return std::make_unique<sol::ForwardMaterialNode>(mtlInstance);
}
