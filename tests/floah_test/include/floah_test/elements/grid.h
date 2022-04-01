#pragma once

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "bettertest/mixins/compare_mixin.h"
#include "bettertest/mixins/exception_mixin.h"
#include "bettertest/tests/unit_test.h"

namespace test
{
    class GridProperties final : public bt::UnitTest<GridProperties, bt::CompareMixin>
    {
    public:
        void operator()() override;
    };

    class GridElements final : public bt::UnitTest<GridElements, bt::CompareMixin, bt::ExceptionMixin>
    {
    public:
        void operator()() override;
    };

    class GridAppendRow final : public bt::UnitTest<GridAppendRow, bt::CompareMixin, bt::ExceptionMixin>
    {
    public:
        void operator()() override;
    };

    class GridPrependRow final : public bt::UnitTest<GridPrependRow, bt::CompareMixin, bt::ExceptionMixin>
    {
    public:
        void operator()() override;
    };

    class GridInsertRow final : public bt::UnitTest<GridInsertRow, bt::CompareMixin, bt::ExceptionMixin>
    {
    public:
        void operator()() override;
    };

    class GridRemoveRow final : public bt::UnitTest<GridRemoveRow, bt::CompareMixin, bt::ExceptionMixin>
    {
    public:
        void operator()() override;
    };

    class GridExtractRow final : public bt::UnitTest<GridExtractRow, bt::CompareMixin, bt::ExceptionMixin>
    {
    public:
        void operator()() override;
    };

    class GridAppendColumn final : public bt::UnitTest<GridAppendColumn, bt::CompareMixin, bt::ExceptionMixin>
    {
    public:
        void operator()() override;
    };

    class GridPrependColumn final : public bt::UnitTest<GridPrependColumn, bt::CompareMixin, bt::ExceptionMixin>
    {
    public:
        void operator()() override;
    };

    class GridInsertColumn final : public bt::UnitTest<GridInsertColumn, bt::CompareMixin, bt::ExceptionMixin>
    {
    public:
        void operator()() override;
    };

    class GridRemoveColumn final : public bt::UnitTest<GridRemoveColumn, bt::CompareMixin, bt::ExceptionMixin>
    {
    public:
        void operator()() override;
    };

    class GridExtractColumn final : public bt::UnitTest<GridExtractColumn, bt::CompareMixin, bt::ExceptionMixin>
    {
    public:
        void operator()() override;
    };

    
}  // namespace test
