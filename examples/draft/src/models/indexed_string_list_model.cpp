#include "draft/models/indexed_string_list_model.h"

IndexedStringListModel::IndexedStringListModel() :
    list(std::make_unique<StringListDataSource>()), index(std::make_unique<IndexDataSource>())
{
}
