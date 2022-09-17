#pragma once

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "floah-data/i_bool_data_source.h"
#include "floah-widget/panel.h"

////////////////////////////////////////////////////////////////
// Current target includes.
////////////////////////////////////////////////////////////////

#include "draft/models/indexed_string_list_model.h"

struct GlobalModel
{
    GlobalModel();

    bool                                    val0 = false;
    bool                                    val1 = true;
    std::unique_ptr<floah::IBoolDataSource> val0Source;
    std::unique_ptr<floah::IBoolDataSource> val1Source;
    IndexedStringListModel                  stringList;
};
