#pragma once

#include "app_settings.hpp"
#include "argument.hpp"


struct version : argument<app_settings>
{
    argtype type() const override
    {
        return argtype::toggle;
    }

    std::optional<char> symbol() const override { return 'V'; }
    std::optional<std::string> string() const override { return "version"; }

    std::string_view description() const override
    {
        return "print version and exit";
    }

    void operator()(app_settings& app) override
    {
        app.state = app_state::quit_ok;
        std::cerr << "1.0.1\n";
    }
};


