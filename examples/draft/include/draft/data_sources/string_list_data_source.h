#pragma once

////////////////////////////////////////////////////////////////
// Standard includes.
////////////////////////////////////////////////////////////////

#include <string>
#include <vector>

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "floah-data/i_list_data_source.h"

class StringListDataSource : public floah::IListDataSource
{
public:
    ////////////////////////////////////////////////////////////////
    // Constructors.
    ////////////////////////////////////////////////////////////////

    StringListDataSource() = default;

    StringListDataSource(const StringListDataSource&) = delete;

    StringListDataSource(StringListDataSource&&) noexcept = delete;

    ~StringListDataSource() noexcept override = default;

    StringListDataSource& operator=(const StringListDataSource&) = delete;

    StringListDataSource& operator=(StringListDataSource&&) noexcept = delete;

    ////////////////////////////////////////////////////////////////
    // Getters.
    ////////////////////////////////////////////////////////////////

    [[nodiscard]] size_t getSize() override;

    [[nodiscard]] void* getVoid(size_t i) override;

    [[nodiscard]] std::string getString(size_t i) override;

    ////////////////////////////////////////////////////////////////
    // Setters.
    ////////////////////////////////////////////////////////////////

    void appendVoid(void* v) override;

    void appendString(const std::string& v) override;

    void prependVoid(void* v) override;

    void prependString(const std::string& v) override;

    void insertVoid(void* v, size_t i) override;

    void insertString(const std::string& v, size_t i) override;

    void setVoid(void* v, size_t i) override;

    void setString(const std::string& v, size_t i) override;

    void remove(size_t i) override;

protected:
    std::vector<std::string> list;
};
