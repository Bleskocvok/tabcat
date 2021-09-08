#pragma once

#include <vector>
#include <memory>
#include <algorithm>    // find_if
#include <stdexcept>    // out_of_range

#include "include/printer.hpp"
#include "include/parser.hpp"
#include "argument.hpp"
#include "io.hpp"


enum class app_state
{
    cont, quit_ok, quit_error
};


struct app_settings
{
    std::unique_ptr<abs_input> input = std::make_unique<std_input>();
    std::unique_ptr<abs_output> output = std::make_unique<std_output>();
    parser parse;
    printer<std::ostream> print{ output->ostream() };
    arguments<app_settings> args;
    std::string program_name;
    app_state state = app_state::cont;
};
