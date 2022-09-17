#include "draft/data_sources/index_data_source.h"

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

void IndexDataSource::setIndex(const int64_t v)
{
    index = v;
    emitDataSourceUpdate();
}

void IndexDataSource::setVoid(void* v) { throw std::runtime_error(""); }

void IndexDataSource::setString(const std::string& v) { throw std::runtime_error(""); }

void IndexDataSource::setIntegral(const int8_t v)
{
    index = static_cast<decltype(index)>(v);
    emitDataSourceUpdate();
}

void IndexDataSource::setIntegral(const int16_t v)
{
    index = static_cast<decltype(index)>(v);
    emitDataSourceUpdate();
}

void IndexDataSource::setIntegral(const int32_t v)
{
    index = static_cast<decltype(index)>(v);
    emitDataSourceUpdate();
}

void IndexDataSource::setIntegral(const int64_t v)
{
    index = static_cast<decltype(index)>(v);
    emitDataSourceUpdate();
}

void IndexDataSource::setIntegral(const uint8_t v)
{
    index = static_cast<decltype(index)>(v);
    emitDataSourceUpdate();
}

void IndexDataSource::setIntegral(const uint16_t v)
{
    index = static_cast<decltype(index)>(v);
    emitDataSourceUpdate();
}

void IndexDataSource::setIntegral(const uint32_t v)
{
    index = static_cast<decltype(index)>(v);
    emitDataSourceUpdate();
}

void IndexDataSource::setIntegral(const uint64_t v)
{
    index = static_cast<decltype(index)>(v);
    emitDataSourceUpdate();
}
