#include "draft/panels/simple_panel.h"

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "common/enum_classes.h"
#include "floah-viz/generators/rectangle_generator.h"
#include "floah-widget/widgets/dropdown.h"
#include "floah-widget/widgets/radio_button.h"
#include "sol/scenegraph/forward/forward_material_node.h"
#include "sol/scenegraph/drawable/mesh_node.h"

////////////////////////////////////////////////////////////////
// Constructors.
////////////////////////////////////////////////////////////////

SimplePanel::SimplePanel(floah::InputContext& context) : Panel(context) {}

SimplePanel::~SimplePanel() noexcept = default;

////////////////////////////////////////////////////////////////
// Getters.
////////////////////////////////////////////////////////////////

sol::Node* SimplePanel::getPanelNode() noexcept
{
    return nodes.widgetRootNode ? &nodes.widgetRootNode->getAsNode() : nullptr;
}

const sol::Node* SimplePanel::getPanelNode() const noexcept
{
    return nodes.widgetRootNode ? &nodes.widgetRootNode->getAsNode() : nullptr;
}

////////////////////////////////////////////////////////////////
// Init.
////////////////////////////////////////////////////////////////

void SimplePanel::initialize(
  const int32_t xoffset, const int32_t yoffset, const int32_t width, const int32_t height, const GlobalModel& model)
{
    offset = {xoffset, yoffset};

    createLayer("background", -1);
    auto& topLayer = createLayer("top", 1);

    layout->getSize().setWidth(floah::Length(width));
    layout->getSize().setHeight(floah::Length(height));

    auto& root = layout->setRoot(std::make_unique<floah::VerticalFlow>());
    root.getSize().setWidth(floah::Length(1.0f));
    root.getSize().setHeight(floah::Length(1.0f));
    auto& dropdownElem = root.append(std::make_unique<floah::LayoutElement>());
    dropdownElem.getSize().setWidth(floah::Length(1.0f));
    dropdownElem.getSize().setHeight(floah::Length(32));
    auto& checkbox0Elem = root.append(std::make_unique<floah::LayoutElement>());
    checkbox0Elem.getSize().setWidth(floah::Length(1.0f));
    checkbox0Elem.getSize().setHeight(floah::Length(32));
    auto& checkbox1Elem = root.append(std::make_unique<floah::LayoutElement>());
    checkbox1Elem.getSize().setWidth(floah::Length(1.0f));
    checkbox1Elem.getSize().setHeight(floah::Length(32));

    auto& dropdown = addWidget(std::make_unique<floah::Dropdown>(), topLayer);
    dropdown.setLabel("A Value");
    dropdown.setPanelLayoutElement(dropdownElem);
    dropdown.setItemsDataSource(model.stringList.list.get());
    dropdown.setIndexDataSource(model.stringList.index.get());

    auto& checkbox0 = addWidget(std::make_unique<floah::RadioButton>(), topLayer);
    checkbox0.setLabel("Some Toggle");
    checkbox0.setPanelLayoutElement(checkbox0Elem);
    checkbox0.setDataSource(model.val0Source.get());

    auto& checkbox1 = addWidget(std::make_unique<floah::RadioButton>(checkbox0), topLayer);
    checkbox1.setLabel("Another Toggle");
    checkbox1.setPanelLayoutElement(checkbox1Elem);
    checkbox1.setDataSource(model.val1Source.get());

    generatePanelLayout();
    generateWidgetLayouts();
}

////////////////////////////////////////////////////////////////
// Generate.
////////////////////////////////////////////////////////////////

void SimplePanel::generateGeometry(sol::MeshManager& meshManager, floah::FontMap& fontMap)
{
    Panel::generateGeometry(meshManager, fontMap);

    if (none(staleData & StaleData::Geometry)) return;

    // TODO: Update meshes if they already exist.
    floah::Generator::Params params{.meshManager = meshManager, .fontMap = fontMap};
    {
        floah::RectangleGenerator gen;
        gen.lower         = 0;
        gen.upper         = math::float2(layout->getSize().getWidth().get(), layout->getSize().getHeight().get());
        gen.fillMode      = floah::RectangleGenerator::FillMode::Fill;
        gen.color         = math::float4(0.1f, 0.1f, 0.25f, 1.0f);
        meshes.background = &gen.generate(params);
    }

    {
        floah::RectangleGenerator gen;
        gen.lower      = 0;
        gen.upper      = math::float2(layout->getSize().getWidth().get(), layout->getSize().getHeight().get());
        gen.fillMode   = floah::RectangleGenerator::FillMode::Outline;
        gen.margin     = floah::Length(2);
        gen.color      = math::float4(1.0f);
        meshes.outline = &gen.generate(params);
    }

    staleData = staleData & ~StaleData::Geometry;
}

void SimplePanel::generateScenegraph(floah::IScenegraphGenerator& generator)
{
    if (any(staleData & StaleData::Scenegraph))
    {
        if (!meshes.background) throw std::runtime_error("Cannot generate scenegraph. Geometry was not generated yet.");

        if (!nodes.root)
        {
            nodes.root = &generator.createPanelNode(nullptr);

            nodes.widgetRootNode = &generator.createPanelTransformNode(
              *nodes.root, math::float3(offset.x, offset.y, -2));  // TODO: Set proper z.

            auto& panelMtlNode = nodes.root->addChild(std::make_unique<sol::ForwardMaterialNode>());
            panelMtlNode.setMaterial(getPanelMaterial());



            auto& panelTansformNode =
              generator.createPanelTransformNode(panelMtlNode, math::float3(offset.x, offset.y, -2))
                .getAsNode();  // TODO: Set proper z.

            auto& widgetTransformNode = generator.createWidgetTransformNode(panelTansformNode, math::float3(0));

            widgetTransformNode.getAsNode().addChild(std::make_unique<sol::MeshNode>(*meshes.outline));
            widgetTransformNode.getAsNode().addChild(std::make_unique<sol::MeshNode>(*meshes.background));
        }
        else
        {
            // TODO: Update nodes if they already exist.
        }

        staleData = staleData & ~StaleData::Scenegraph;
    }

    Panel::generateScenegraph(generator);
}

////////////////////////////////////////////////////////////////
// Input.
////////////////////////////////////////////////////////////////

math::int2 SimplePanel::getInputOffset() const noexcept { return offset; }

////////////////////////////////////////////////////////////////
// Stylesheet getters.
////////////////////////////////////////////////////////////////

sol::ForwardMaterialInstance* SimplePanel::getPanelMaterial() const noexcept
{
    const auto mtl = getStylesheetProperty<sol::ForwardMaterialInstance*>(material_panel);
    if (mtl) return *mtl;
    return nullptr;
}