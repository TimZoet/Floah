#include <iostream>

#include "bettertest/run.h"
#include "floah_test/elements/grid.h"
#include "floah_test/elements/horizontal_flow.h"

#ifdef WIN32
#include "Windows.h"
#endif

int main(int argc, char** argv)
{
    // Set path next to executable.
#ifdef WIN32
    {
        WCHAR path[MAX_PATH];
        GetModuleFileNameW(NULL, path, MAX_PATH);
        std::filesystem::path workdir(path);
        const auto            p = workdir.parent_path();
        std::filesystem::current_path(p);
    }
#endif
    bt::run<test::GridProperties,
            test::GridElements,
            test::GridAppendRow,
            test::GridPrependRow,
            test::GridInsertRow,
            test::GridRemoveRow,
            test::GridExtractRow,
            test::GridAppendColumn,
            test::GridPrependColumn,
            test::GridInsertColumn,
            test::GridRemoveColumn,
            test::GridExtractColumn,
            test::HorizontalFlowProperties,
            test::HorizontalFlowElements>(argc, argv, "floah");
    return 0;
}
