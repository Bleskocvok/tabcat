#include <iostream>
#include <string>       // getline
#include <algorithm>    // max, all_of
#include <iomanip>      // setw
#include <clocale>      // set_locale, mbstate_t
#include <cwchar>       // mbrlen
#include <cctype>       // isdigit
#include <vector>
#include <string_view>


#include "classes.hpp"


size_t unicode_size(std::string_view str)
{
    std::mbstate_t mb{};
    size_t size;
    for (size = 0; !str.empty(); size++)
    {
        size_t ch_size = std::mbrlen(&str.front(), str.size(), &mb);
        str.remove_prefix(ch_size);
    }
    return size;
}


size_t size_diff(std::string_view str)
{
    return unicode_size(str) - str.length();
}


std::vector<std::string> parse_lines(std::istream& in)
{
    std::vector<std::string> result;
    while (in)
    {
        std::string line;
        std::getline(in, line);
        result.push_back(std::move(line));
    }
    return result;
}


struct location
{
    bool first, last;
};

template<typename Func>
void for_each_cell(const std::vector<std::string>& lines
                    , Func func
                    , std::string_view del = ";")
{
    size_t row = 0;
    for (std::string_view l : lines)
    {
        size_t pos = 0;
        size_t col;
        for (col = 0; (pos = l.find(del)) != std::string_view::npos; col++)
        {
            func(row, col, l.substr(0, pos), location{ col == 0, false });
            l.remove_prefix(pos + 1);
        }
        func(row, col, l, location{ col == 0, true });
        row++;
    }
}


void print_header(std::ostream& out
                , const std::vector<size_t>& sizes
                , std::string begin
                , std::string mid
                , std::string join
                , std::string end)
{
    std::string prefix = begin + mid;
    for (size_t s : sizes)
    {
        out << prefix;
        for (size_t i = 0; i < s; i++)
        {
            out << "─";
        }
        prefix = mid + join + mid;
    }
    out << mid << end << "\n";
}


bool is_numeric(std::string_view str)
{
    return std::all_of(str.begin(), str.end(), [](auto ch)
    {
        return std::isdigit(ch);
    });
}


void print_table(std::ostream& out
                , const std::vector<std::string>& data
                , const std::vector<size_t>& sizes
                , std::string_view del = ";")
{
    print_header(out, sizes, "┌", "─", "┬", "┐");

    auto print_cell = [&](size_t row, size_t col, auto str, location loc)
    {
        out << (loc.first ? "" : " ") << "│ ";
        out << (is_numeric(str) ? std::right : std::left)
            << std::setw(sizes[col] - size_diff(str))
            << str;
        if (loc.last)
            out << " │\n";
        if (row == 0 && loc.last)
            print_header(out, sizes, "├", "─", "┼", "┤");
    };
    for_each_cell(data, print_cell, del);

    print_header(out, sizes, "└", "─", "┴", "┘");
}


std::vector<std::string> load_lines(std::istream& in)
{
    std::vector<std::string> res;
    std::string line;
    while (in)
    {
        std::getline(in, line);
        res.push_back(std::move(line));
    }
    while (!res.empty() && res.back().empty())
    {
        res.pop_back();
    }
    return res;
}


int main([[maybe_unused]]int argc, [[maybe_unused]]char** argv)
{
    std::setlocale(LC_ALL, "en_US.utf8");

    auto input = load_lines(std::cin);

    std::vector<size_t> sizes(1, 0);
    auto get_max = [&](size_t /* row */, size_t col, std::string_view str, auto)
    {
        sizes.resize(std::max(col + 1, sizes.size()));
        size_t size = unicode_size(str);
        sizes[col] = std::max(sizes[col], size);
    };

    for_each_cell(input, get_max);

    print_table(std::cout, input, sizes);

    return 0;
}


table parse(std::istream& in)
{
    const char eol = '\n';

    std::string line;
    while (in)
    {
        std::getline(in, line, eol);
        column col;
    }
    return table{};
}
