#pragma once

#include "app_settings.hpp"
#include "argument.hpp"


struct version : argument<app_settings>
{
    argument::argtype type() const override
    {
        return argument::argtype::toggle;
    }

    std::string_view symbol() const override { return "V"; }
    std::string_view string() const override { return "version"; }

    std::string_view description() const override
    {
        return "print version and exit";
    }

    void operator()(app_settings& app) override
    {
        app.state = app_state::quit_ok;
        auto& out = std::cerr;
        out << "0.8.0-alpha\n";
    }
};


