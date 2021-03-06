#pragma once

#include "app_settings.hpp"
#include "argument.hpp"


struct delimiter : argument<app_settings>
{
    argtype type() const override
    {
        return argtype::next_arg_value;
    }

    std::optional<char> symbol() const override { return 'd'; }
    std::optional<std::string> string() const override { return "delimiter"; }

    std::string_view description() const override
    {
        return "set the character separating input values";
    }

    void operator()(app_settings& app, std::string_view val) override
    {
        app.parse.del = val;
    }
};


