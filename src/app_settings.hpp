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
    std::vector<std::unique_ptr<argument<app_settings>>> options;
    std::string program_name;
    app_state state = app_state::cont;

    app_settings(std::ostream& out)
        : print(out) {}

    template<typename Pred>
    argument<app_settings>* find_opt(Pred pred)
    {
        auto it = std::find_if(options.begin(), options.end(), pred);

        if (it == options.end())
            return nullptr;

        return it->get();
    }

    argument<app_settings>* opt_by_symbol(std::string_view sym)
    {
        return find_opt([=](const auto& opt){ return opt->symbol() == sym; });
    }

    argument<app_settings>* opt_by_string(std::string_view str)
    {
        return find_opt([=](const auto& opt){ return opt->string() == str; });
    }
};
