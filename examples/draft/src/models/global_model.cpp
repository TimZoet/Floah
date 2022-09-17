#include "draft/models/global_model.h"

////////////////////////////////////////////////////////////////
// Current target includes.
////////////////////////////////////////////////////////////////

#include "draft/data_sources/bool_data_source.h"

////////////////////////////////////////////////////////////////
// Constructors.
////////////////////////////////////////////////////////////////

GlobalModel::GlobalModel()
{
    val0Source = std::make_unique<BoolDataSource>(val0);
    val1Source = std::make_unique<BoolDataSource>(val1);

    stringList.list->appendString("0000");
    stringList.list->appendString("0001");
    stringList.list->appendString("0002");
    stringList.list->appendString("0003");
    stringList.list->appendString("0004");
    stringList.list->appendString("0005");
    stringList.list->appendString("0006");
    stringList.list->appendString("0007");
    stringList.list->appendString("0008");
    stringList.list->appendString("0009");
}
