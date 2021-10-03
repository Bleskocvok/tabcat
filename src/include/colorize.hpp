#pragma once


#include <vector>
#include <string>
#include <string_view>


inline static std::string col_begin = "\x1b[";
inline static std::string col_end = "m";
inline static std::string del = ";";
inline static std::string reset = "\x1b[0m";


enum class decoration
{
    none    = 0,
    bold    = 1,
};


enum class fg_color
{
    black   = 30,
    red     = 31,
    green   = 32,
    yellow  = 33,
    blue    = 34,
    magenta = 35,
    cyan    = 36,
    white   = 37,
};


// TODO
struct truecolor
{
    uint8_t r = 255,
            g = 255,
            b = 255;
};


template<typename T>
constexpr std::string str_enum(T value)
{
    return std::to_string(static_cast<int>(value));
}


std::string colorize(std::string str, fg_color color)
{
    auto result = std::string{};
    result += col_begin;
    result += str_enum(color);
    result += col_end;
    result += str;
    result += reset;
    return result;
}
