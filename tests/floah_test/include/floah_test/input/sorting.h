#pragma once

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "bettertest/mixins/compare_mixin.h"
#include "bettertest/tests/unit_test.h"

namespace test
{
    class InputElementSorting final : public bt::UnitTest<InputElementSorting, bt::CompareMixin>
    {
    public:
        void operator()() override;
    };
}  // namespace test
