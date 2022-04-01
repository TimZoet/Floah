#pragma once

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "bettertest/mixins/compare_mixin.h"
#include "bettertest/mixins/exception_mixin.h"
#include "bettertest/tests/unit_test.h"

namespace test
{
    class HorizontalFlowProperties final : public bt::UnitTest<HorizontalFlowProperties, bt::CompareMixin>
    {
    public:
        void operator()() override;
    };

    class HorizontalFlowElements final : public bt::UnitTest<HorizontalFlowElements, bt::CompareMixin, bt::ExceptionMixin>
    {
    public:
        void operator()() override;
    };
}  // namespace test
