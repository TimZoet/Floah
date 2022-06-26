#include "draft/generators.h"

////////////////////////////////////////////////////////////////
// Current target includes.
////////////////////////////////////////////////////////////////

#include "draft/materials/widget_material.h"

////////////////////////////////////////////////////////////////
// Constructors.
////////////////////////////////////////////////////////////////

ScenegraphGenerator::ScenegraphGenerator() = default;

ScenegraphGenerator::ScenegraphGenerator(sol::ForwardMaterial& wdgtMaterial,
                                         sol::ForwardMaterial& txtMaterial,
                                         sol::Node&            wdgtNode,
                                         sol::Node&            txtNode) :
    widgetMaterial(&wdgtMaterial), textMaterial(&txtMaterial), widgetNode(&wdgtNode), textNode(&txtNode)
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

sol::Node& ScenegraphGenerator::createWidgetNode(const math::float3 offset)
{
    return widgetNode->addChild(std::make_unique<WidgetTransformNode>(*widgetMaterial, math::float4(offset, 1.0f)));
}

sol::Node& ScenegraphGenerator::createTextNode(const math::float3 offset)
{
    return textNode->addChild(std::make_unique<WidgetTransformNode>(*textMaterial, math::float4(offset, 1.0f)));
}
