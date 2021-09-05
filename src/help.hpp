#pragma once

#include <iomanip>      // setw
#include <string>

#include "app_settings.hpp"
#include "argument.hpp"


struct help : argument<app_settings>
{
    argument::argtype type() const override
    {
        return argument::argtype::toggle;
    }

    std::optional<std::string> string() const override { return "help"; }

    std::string_view description() const override
    {
        return "print this help and quit";
    }

    void operator()(app_settings& /*app*/, std::string_view /*val*/) override
    {

    }

    void operator()(app_settings& app) override
    {
        using namespace std::string_literals;

        app.state = app_state::quit_ok;

        auto& out = std::cerr;

        out << "Usage: " << app.program_name << " [OPTION]...\n";

        for (const auto& opt : app.options)
        {
            out << "  ";

            std::string symbol = "  ";
            symbol += !opt->symbol() ? ""
                                     : "-"s + *opt->symbol();
            if (opt->symbol() && opt->string())
                symbol += ",";

            std::string str;
            str += !opt->string() ? ""
                                  : "--"s + *opt->string();

            if (opt->type() == argument::argtype::enum_value)
            {
                str += "=PARAM";
            }

            if (opt->type() == argument::argtype::next_arg_value)
            {
                str += " PARAM";
            }

            out << std::left
                << std::setw(6) << symbol
                << std::setw(22) << str << " "
                << opt->description() << "\n";
        }
    }
};

