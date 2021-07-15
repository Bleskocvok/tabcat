#pragma once

#include <string_view>


namespace color
{


inline std::string_view reset   = "\033[0m",
                        black   = "\033[0;30m",
                        white   = "\033[1;37m",
                        red     = "\033[0;31m",
                        green   = "\033[0;32m",
                        blue    = "\033[0;34m";


} // namespace color


// nice looking style below

// if (loc.row % 2 == 0)
// {
//     // out << color::black;
//     // out << "\033[48;2;232;194;882m";
//     out << "\033[7;49;37m";
// }
// else
// {
//     // out << color::black;
//     // out << "\033[48;2;168;137;502m";
//     out << "\033[7;49;97m";
// }
