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

    // std::string_view symbol() const override { return "h"; }
    std::string_view string() const override { return "help"; }

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
        using s = std::string;

        app.state = app_state::quit_ok;

        auto& out = std::cerr;

        out << "Usage: " << app.program_name << " [OPTION]...\n";

        for (const auto& opt : app.options)
        {
            out << "  ";

            std::string symbol = "  ";
            symbol += opt->symbol().empty() ? ""
                                            : "-"s + s(opt->symbol());
            if (!opt->symbol().empty() && !opt->string().empty())
                symbol += ",";

            std::string str;
            str += opt->string().empty() ? ""
                                         : "--"s + s(opt->string());

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

