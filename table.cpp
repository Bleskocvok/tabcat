#include <clocale>      // set_locale

#include <iostream>
#include <string>       // getline
#include <iomanip>      // setw

#include <algorithm>    // max, all_of
#include <utility>      // move
#include <vector>
#include <string_view>


#include "classes.hpp"
#include "unicode.hpp"


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


void print_table(std::ostream& out, table& tab)
{
    auto sizes = tab.layout();

    print_header(out, sizes, "┌", "─", "┬", "┐");

    auto print_cell = [&](auto str, location loc)
    {
        out << (loc.first_col ? "" : " ") << "│ ";
        out << (is_numeric(str) ? std::right : std::left)
            << std::setw(sizes[loc.col] - size_diff(str))
            << str;

        if (loc.last_col)
            out << " │\n";

        if (loc.row == 0 && loc.last_col)
            print_header(out, sizes, "├", "─", "┼", "┤");
    };
    tab.for_each_cell(print_cell);

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


template<typename Func>
void parse_each_cell(std::istream& in
                    , Func func
                    , std::string_view del = ";"
                    , char eol = '\n')
{
    size_t row = 0;
    std::string line;
    while (in)
    {
        std::getline(in, line, eol);
        std::string_view l = line;

        size_t pos = 0;
        size_t col;
        for (col = 0; (pos = l.find(del)) != std::string_view::npos; col++)
        {
            func(l.substr(0, pos), col, row);
            l.remove_prefix(pos + 1);
        }
        func(l, col, row);
        row++;
    }
}


int main([[maybe_unused]]int argc, [[maybe_unused]]char** argv)
{
    std::setlocale(LC_ALL, "en_US.utf8");

    table tab = table::parse(std::cin);

    print_table(std::cout, tab);

    // auto input = load_lines(std::cin);

    // std::vector<size_t> sizes(1, 0);
    // auto get_max = [&](size_t /* row */, size_t col, std::string_view str, auto)
    // {
    //     sizes.resize(std::max(col + 1, sizes.size()));
    //     size_t size = unicode_size(str);
    //     sizes[col] = std::max(sizes[col], size);
    // };

    // for_each_cell(input, get_max);

    // print_table(std::cout, input, sizes);

    return 0;
}
