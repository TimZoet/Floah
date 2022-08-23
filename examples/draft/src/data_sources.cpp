#include "draft/data_sources.h"

////////////////////////////////////////////////////////////////
// Standard includes.
////////////////////////////////////////////////////////////////

#include <stdexcept>

////////////////////////////////////////////////////////////////
// Getters.
////////////////////////////////////////////////////////////////

[[nodiscard]] int64_t IndexDataSource::getIndex() const noexcept { return index; }

void* IndexDataSource::getVoid() { throw std::runtime_error(""); }

std::string IndexDataSource::getString() { return std::to_string(index); }

void IndexDataSource::getIntegral(int8_t& v) { v = static_cast<std::remove_reference_t<decltype(v)>>(index); }
void IndexDataSource::getIntegral(int16_t& v) { v = static_cast<std::remove_reference_t<decltype(v)>>(index); }
void IndexDataSource::getIntegral(int32_t& v) { v = static_cast<std::remove_reference_t<decltype(v)>>(index); }
void IndexDataSource::getIntegral(int64_t& v) { v = index; }
void IndexDataSource::getIntegral(uint8_t& v) { v = static_cast<std::remove_reference_t<decltype(v)>>(index); }
void IndexDataSource::getIntegral(uint16_t& v) { v = static_cast<std::remove_reference_t<decltype(v)>>(index); }
void IndexDataSource::getIntegral(uint32_t& v) { v = static_cast<std::remove_reference_t<decltype(v)>>(index); }
void IndexDataSource::getIntegral(uint64_t& v) { v = static_cast<std::remove_reference_t<decltype(v)>>(index); }

////////////////////////////////////////////////////////////////
// Setters.
////////////////////////////////////////////////////////////////

void IndexDataSource::setIndex(const int64_t v) { index = v; }

void IndexDataSource::setVoid(void* v) { throw std::runtime_error(""); }

void IndexDataSource::setString(const std::string& v) { throw std::runtime_error(""); }

void IndexDataSource::setIntegral(const int8_t v) { index = static_cast<decltype(index)>(v); }
void IndexDataSource::setIntegral(const int16_t v) { index = static_cast<decltype(index)>(v); }
void IndexDataSource::setIntegral(const int32_t v) { index = static_cast<decltype(index)>(v); }
void IndexDataSource::setIntegral(const int64_t v) { index = static_cast<decltype(index)>(v); }
void IndexDataSource::setIntegral(const uint8_t v) { index = static_cast<decltype(index)>(v); }
void IndexDataSource::setIntegral(const uint16_t v) { index = static_cast<decltype(index)>(v); }
void IndexDataSource::setIntegral(const uint32_t v) { index = static_cast<decltype(index)>(v); }
void IndexDataSource::setIntegral(const uint64_t v) { index = static_cast<decltype(index)>(v); }

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

void StringListDataSource::appendString(const std::string& v) { list.emplace_back(v); }

void StringListDataSource::prependVoid(void* v) { throw std::runtime_error(""); }

void StringListDataSource::prependString(const std::string& v) { list.insert(list.begin(), v); }

void StringListDataSource::insertVoid(void* v, size_t i) { throw std::runtime_error(""); }

void StringListDataSource::insertString(const std::string& v, const size_t i) { list.insert(list.begin() + i, v); }

void StringListDataSource::setVoid(void* v, size_t i) { throw std::runtime_error(""); }

void StringListDataSource::setString(const std::string& v, size_t i) { list[i] = v; }

void StringListDataSource::remove(const size_t i) { list.erase(list.begin() + i); }

IndexedStringList::IndexedStringList() :
    list(std::make_unique<StringListDataSource>()), index(std::make_unique<IndexDataSource>())
{
}
