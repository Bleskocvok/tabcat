#include <clocale>      // set_locale

#include <iostream>
#include <string>       // getline
#include <iomanip>      // setw

#include <algorithm>    // max, all_of
#include <utility>      // move
#include <vector>
#include <string_view>


#include "table.hpp"
#include "unicode.hpp"
#include "printer.hpp"


int main([[maybe_unused]]int argc, [[maybe_unused]]char** argv)
{
    set_locale_all("en_US.utf8");

    table tab = table::parse(std::cin);
    printer pr(std::cout);
    pr.print(tab);

    return 0;
}
