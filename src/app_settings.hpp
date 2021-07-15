#pragma once

#include <ostream>
#include <vector>
#include <memory>

#include "include/printer.hpp"
#include "include/parser.hpp"
#include "argument.hpp"


struct app_settings
{
    parser parse;
    printer<std::ostream> print;
    std::vector<std::unique_ptr<argument<app_settings>>> options;

    app_settings(std::ostream& out)
        : print(out) {}
};
