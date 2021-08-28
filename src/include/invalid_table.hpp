#pragma once

#include <stdexcept>    // runtime_error
#include <string>


struct invalid_table : std::runtime_error
{
    explicit invalid_table(const std::string& str)
        : std::runtime_error(str) {}

    explicit invalid_table(const char* str)
        : std::runtime_error(str) {}
};
