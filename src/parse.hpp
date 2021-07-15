#pragma once

#include <istream>
#include <string>       // getline
#include <cctype>       // isspace
#include <string_view>


inline bool is_numeric(std::string_view str)
{
    return std::all_of(str.begin(), str.end(), [](unsigned char ch)
    {
        return std::isdigit(ch);
    });
}


inline void getline_to_str(std::istream& in
                    , std::string& out
                    , std::string_view eol = "\n");


template<typename CellFunc>
inline void parse_line(std::istream& in
                , std::string_view del
                , std::string_view eol
                , CellFunc func);


inline void trim(std::string& str);


template<typename CellFunc>
void parse_line(std::istream& in
                , std::string_view del
                , std::string_view eol
                , CellFunc func)
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
        l.remove_prefix(pos + 1);
    }
    func(l, col);
}


void trim(std::string& str)
{
    if (str.empty())
        return;

    while (std::isspace(str.back()))
    {
        str.pop_back();
    }

    size_t last;
    for (last = 0; last < str.length(); last++)
    {
        if (!std::isspace(static_cast<unsigned char>(str[last])))
        {
            break;
        }
    }

    str.erase(0, last);
}


void getline_to_str(std::istream& in
                    , std::string& out
                    , std::string_view eol)
{
    std::string l;
    // getline until line ends with eol
    while (in && l.size() - eol.size() <= l.size()
              && l.compare(l.size() - eol.size(), std::string::npos, eol));
    {
        std::getline(in, l, eol.back());
        out += std::move(l);
    }
}
