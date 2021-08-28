#pragma once

#include "app_settings.hpp"
#include "argument.hpp"


struct delimiter : argument<app_settings>
{
    argument::argtype type() const override
    {
        return argument::argtype::next_arg_value;
    }

    std::string_view symbol() const override { return "d"; }
    std::string_view string() const override { return "delimiter"; }

    std::string_view description() const override
    {
        return "set the character separating input values";
    }

    void operator()(app_settings& app, std::string_view val) override
    {
        app.parse.del = val;
    }
};


