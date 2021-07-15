#pragma once

#include "app_settings.hpp"
#include "argument.hpp"


struct disable_top : argument<app_settings>
{
    argument::argtype type() const { return argument::argtype::toggle; }

    std::string_view symbol() const { return "-T"; }
    std::string_view string() const { return ""; }

    void operator()(app_settings& app) override
    {
        app.print.style.disable(border_pos::top);
    }
};




struct test : argument<app_settings>
{
    argument::argtype type() const { return argument::argtype::enum_value; }

    std::string_view symbol() const { return ""; }
    std::string_view string() const { return "--test="; }

    void operator()(app_settings&, std::string_view value) override
    {
        std::cout << "test: " << value << std::endl;
    }
};
