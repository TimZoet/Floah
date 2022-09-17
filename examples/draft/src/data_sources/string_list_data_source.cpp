#include "draft/data_sources/string_list_data_source.h"

////////////////////////////////////////////////////////////////
// Standard includes.
////////////////////////////////////////////////////////////////

#include <stdexcept>

////////////////////////////////////////////////////////////////
// Getters.
////////////////////////////////////////////////////////////////

size_t StringListDataSource::getSize() { return list.size(); }

void* StringListDataSource::getVoid(const size_t) { throw std::runtime_error(""); }

std::string StringListDataSource::getString(const size_t i) { return list[i]; }

////////////////////////////////////////////////////////////////
// Setters.
////////////////////////////////////////////////////////////////

void StringListDataSource::appendVoid(void* v) { throw std::runtime_error(""); }

void StringListDataSource::appendString(const std::string& v)
{
    list.emplace_back(v);
    emitDataSourceUpdate();
}

void StringListDataSource::prependVoid(void* v) { throw std::runtime_error(""); }

void StringListDataSource::prependString(const std::string& v)
{
    list.insert(list.begin(), v);
    emitDataSourceUpdate();
}

void StringListDataSource::insertVoid(void* v, size_t i) { throw std::runtime_error(""); }

void StringListDataSource::insertString(const std::string& v, const size_t i)
{
    list.insert(list.begin() + i, v);
    emitDataSourceUpdate();
}

void StringListDataSource::setVoid(void* v, size_t i) { throw std::runtime_error(""); }

void StringListDataSource::setString(const std::string& v, size_t i)
{
    list[i] = v;
    emitDataSourceUpdate();
}

void StringListDataSource::remove(const size_t i)
{
    list.erase(list.begin() + i);
    emitDataSourceUpdate();
}
