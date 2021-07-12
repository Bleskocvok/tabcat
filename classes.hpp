#pragma once

#include <iostream>
#include <string>       // getline, to_string
#include <algorithm>    // find_if, max
#include <iterator>     // distance
#include <vector>
#include <optional>
#include <string_view>

#include "unicode.hpp"
#include "parse.hpp"


struct invalid_table : std::runtime_error
{
    explicit invalid_table(const std::string& str)
        : std::runtime_error(str) {}

    explicit invalid_table(const char* str)
        : std::runtime_error(str) {}
};


enum class alignment
{
    left, right, center
};


enum column_type
{
    numeric, string
};


struct location
{
    size_t col, row;    // “x”, “y”
    bool first_col, last_col;
};


struct column
{
    std::vector<std::string> data{};
    size_t width{};
    alignment align{};
    column_type type{};
};


struct error
{
    std::string msg{};
};


struct table
{
    std::vector<column> columns{};

    size_t row_count() const
    {
        if (columns.empty())
            return 0;

        return columns.front().data.size();
    }

    size_t column_count() const
    {
        return columns.size();
    }

    const std::string& at(size_t c, size_t r) const
    {
        return columns[c].data[r];
    }

    std::string& at(size_t c, size_t r)
    {
        return columns[c].data[r];
    }

    static table parse(std::istream& in)
    {
        const auto del = std::string_view{ ";" };
        const auto eol = std::string_view{ "\n" };

        using view = std::string_view;

        table result;

        parse_line(std::cin, del, eol, [&](view str, size_t /* col */)
        {
            auto vec = std::vector{ std::string{ std::move(str) } };
            result.columns.push_back(column{ std::move(vec) });
        });

        while (in)
        {
            parse_line(in, del, eol, [&](view str, size_t col)
            {
                if (col >= result.columns.size())
                {
                    throw invalid_table("a row sticking out");
                }

                result.columns[col].data.emplace_back(str);
            });
        }

        auto err = result.check_valid();
        if (err)
        {
            throw invalid_table(err->msg);
        }

        result.setup_width();

        return result;
    }

    [[nodiscard]]
    std::optional<error> check_valid() const
    {
        size_t rows = !columns.empty() ? columns.front().data.size()
                                       : 0;

        auto row_mismatch = std::find_if(columns.begin(), columns.end(),
            [&](const auto& col) { return col.data.size() != rows; });

        if (row_mismatch != columns.end())
        {
            std::string msg = "[ ";
            std::string_view del = "";
            for (const auto& col : columns)
            {
                msg += del;
                msg += std::to_string(col.data.size());
                del = ", ";
            }
            msg += " ]";

            return error
            {
                "uneven number of rows: " + msg
            };
        }

        return std::nullopt;
    }

    template<typename Func>
    void for_each_cell(Func func)
    {
        for (size_t r = 0; r < row_count(); r++)
        {
            for (size_t c = 0; c < column_count(); c++)     // haha c++
            {
                location loc{ c, r, c == 0, c == column_count() - 1 };
                func(at(c, r), loc);
            }
        }
    }

    void setup_width()
    {
        auto set_max = [&](std::string_view str, location loc)
        {
            auto& current = columns[loc.col].width;
            current = std::max(unicode_size(str), current);
        };
        for_each_cell(set_max);
    }

    std::vector<size_t> layout() const
    {
        auto sizes = std::vector<size_t>{};
        sizes.reserve(column_count());
        for (size_t c = 0; c < column_count(); c++)
        {
            sizes.push_back(columns[c].width);
        }
        return sizes;
    }
};