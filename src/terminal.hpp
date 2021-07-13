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
