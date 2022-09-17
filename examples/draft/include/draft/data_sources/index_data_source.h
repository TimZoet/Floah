#pragma once

////////////////////////////////////////////////////////////////
// Standard includes.
////////////////////////////////////////////////////////////////

#include <string>

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "floah-data/i_integral_value_data_source.h"

class IndexDataSource : public floah::IIntegralValueDataSource
{
public:
    ////////////////////////////////////////////////////////////////
    // Constructors.
    ////////////////////////////////////////////////////////////////

    IndexDataSource() = default;

    IndexDataSource(const IndexDataSource&) = delete;

    IndexDataSource(IndexDataSource&&) noexcept = delete;

    ~IndexDataSource() noexcept override = default;

    IndexDataSource& operator=(const IndexDataSource&) = delete;

    IndexDataSource& operator=(IndexDataSource&&) noexcept = delete;

    ////////////////////////////////////////////////////////////////
    // Getters.
    ////////////////////////////////////////////////////////////////

    [[nodiscard]] int64_t getIndex() const noexcept;

    void* getVoid() override;

    std::string getString() override;

protected:
    void getIntegral(int8_t&) override;
    void getIntegral(int16_t&) override;
    void getIntegral(int32_t&) override;
    void getIntegral(int64_t&) override;
    void getIntegral(uint8_t&) override;
    void getIntegral(uint16_t&) override;
    void getIntegral(uint32_t&) override;
    void getIntegral(uint64_t&) override;

    ////////////////////////////////////////////////////////////////
    // Setters.
    ////////////////////////////////////////////////////////////////

public:
    void setIndex(int64_t);

    void setVoid(void* v) override;

    void setString(const std::string& v) override;

protected:
    void setIntegral(int8_t) override;
    void setIntegral(int16_t) override;
    void setIntegral(int32_t) override;
    void setIntegral(int64_t) override;
    void setIntegral(uint8_t) override;
    void setIntegral(uint16_t) override;
    void setIntegral(uint32_t) override;
    void setIntegral(uint64_t) override;

    int64_t index = 0;
};