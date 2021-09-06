#pragma once

#include <ostream>
#include <vector>
#include <memory>
#include <algorithm>    // find_if
#include <stdexcept>    // out_of_range

#include "include/printer.hpp"
#include "include/parser.hpp"
#include "argument.hpp"


enum class app_state
{
    cont, quit_ok, quit_error
};


struct app_settings
{
    parser parse;
    printer<std::ostream> print;
    // std::vector<std::unique_ptr<argument<app_settings>>> options;
    arguments<app_settings> args;
    std::string program_name;
    app_state state = app_state::cont;

    app_settings(std::ostream& out)
        : print(out) {}
};
