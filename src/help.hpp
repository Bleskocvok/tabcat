#pragma once

#include <iomanip>      // setw
#include <string>

#include "app_settings.hpp"
#include "argument.hpp"


struct help : argument<app_settings>
{
    argtype type() const override
    {
        return argtype::toggle;
    }

    std::optional<std::string> string() const override { return "help"; }

    std::string_view description() const override
    {
        return "print this help and quit";
    }

    void operator()(app_settings& /*app*/, std::string_view /*val*/) override
    {
        // TODO: print help for a given option
    }

    void operator()(app_settings& app) override
    {
        app.state = app_state::quit_ok;

        std::cerr << "Usage: " << app.program_name << " [OPTIONS]... [FILE]\n";

        app.args.print_help(std::cerr);
    }
};

