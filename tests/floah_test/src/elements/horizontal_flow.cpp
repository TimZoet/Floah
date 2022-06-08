#include "floah_test/elements/horizontal_flow.h"

#include "floah-layout/elements/horizontal_flow.h"

namespace test
{
    // TODO: None of these tests take place inside of a layout. Adding/removing/extracting children is therefore not tested fully.

    void HorizontalFlowProperties::operator()()
    {
        floah::HorizontalFlow flow;

        // Check initial state.
        compareEQ(flow.getHorizontalAlignment(), floah::HorizontalAlignment::Left);
        compareEQ(flow.getVerticalAlignment(), floah::VerticalAlignment::Top);
        compareEQ(flow.getChildCount(), 0);

        // Modify.
        flow.setHorizontalAlignment(floah::HorizontalAlignment::Right);
        flow.setVerticalAlignment(floah::VerticalAlignment::Middle);

        // Check again.
        compareEQ(flow.getHorizontalAlignment(), floah::HorizontalAlignment::Right);
        compareEQ(flow.getVerticalAlignment(), floah::VerticalAlignment::Middle);

        // Copy.
        floah::HorizontalFlow flow2(flow);
        floah::HorizontalFlow flow3;
        flow3 = flow;
        compareEQ(flow2.getHorizontalAlignment(), floah::HorizontalAlignment::Right);
        compareEQ(flow2.getVerticalAlignment(), floah::VerticalAlignment::Middle);
        compareEQ(flow3.getHorizontalAlignment(), floah::HorizontalAlignment::Right);
        compareEQ(flow3.getVerticalAlignment(), floah::VerticalAlignment::Middle);
    }

    void HorizontalFlowElements::operator()()
    {
        floah::HorizontalFlow flow;

        // Append, prepend and insert some elements.
        compareEQ(flow.getChildCount(), 0);
        auto& elem2 = flow.append(std::make_unique<floah::LayoutElement>());
        compareEQ(flow.getChildCount(), 1);
        auto& elem3 = flow.append(std::make_unique<floah::LayoutElement>());
        compareEQ(flow.getChildCount(), 2);
        auto& elem1 = flow.prepend(std::make_unique<floah::LayoutElement>());
        compareEQ(flow.getChildCount(), 3);
        auto& elem0 = flow.insert(std::make_unique<floah::LayoutElement>(), 0);
        compareEQ(flow.getChildCount(), 4);

        // Check they're in the right spot.
        compareEQ(&flow.get(0), &elem0);
        compareEQ(&flow.get(1), &elem1);
        compareEQ(&flow.get(2), &elem2);
        compareEQ(&flow.get(3), &elem3);
        expectThrow([&] { static_cast<void>(flow.get(4)); });

        // Check parent.
        compareEQ(elem0.getParent(), &flow);
        compareEQ(elem1.getParent(), &flow);
        compareEQ(elem2.getParent(), &flow);
        compareEQ(elem3.getParent(), &flow);

        // Remove.
        expectNoThrow([&] { flow.remove(0); });
        compareEQ(flow.getChildCount(), 3);
        expectNoThrow([&] { flow.remove(2); });
        compareEQ(flow.getChildCount(), 2);
        expectNoThrow([&] { flow.remove(1); });
        compareEQ(flow.getChildCount(), 1);
        expectThrow([&] { flow.remove(1); });
        compareEQ(flow.getChildCount(), 1);

        // Extract.
        const auto elem = flow.extract(0);
        compareNE(elem.get(), nullptr);
        compareEQ(elem->getParent(), nullptr);
        expectThrow([&] { static_cast<void>(flow.extract(0)); });
    }
}  // namespace test
