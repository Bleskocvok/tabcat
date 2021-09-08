#pragma once

#include <string_view>

#include "utils.hpp"


struct parser
{
    std::string_view del = ";";
    std::string_view eol = "\n";

    template<typename CellFunc>
    void parse_line(std::istream& in, CellFunc func)
    {
        std::string line;
        getline_to_str(in, line, eol);

        trim(line);
        if (line.empty())
            return;

        std::string_view l = line;

        size_t pos = 0;
        size_t col;
        for (col = 0; (pos = l.find(del)) != std::string_view::npos; col++)
        {
            func(l.substr(0, pos), col);
            l.remove_prefix(pos + del.length());
        }
        func(l, col);
    }
};
