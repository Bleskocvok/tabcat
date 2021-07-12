#pragma once

#include <vector>
#include <string>
#include <iostream>


enum class alignment
{
    left, right, center
};


enum column_type
{
    numeric, string
};


struct column
{
    std::vector<std::string> data{};
    alignment align{};
    column_type type{};
    size_t width{};
};


struct table
{
    std::vector<column> columns{};
};

