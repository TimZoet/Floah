#pragma once

////////////////////////////////////////////////////////////////
// Standard includes.
////////////////////////////////////////////////////////////////

#include <memory>

////////////////////////////////////////////////////////////////
// Current target includes.
////////////////////////////////////////////////////////////////

#include "draft/data_sources/index_data_source.h"
#include "draft/data_sources/string_list_data_source.h"

struct IndexedStringListModel
{
    std::unique_ptr<StringListDataSource> list;
    std::unique_ptr<IndexDataSource>      index;

    IndexedStringListModel();
};
