#pragma once

#include "app_settings.hpp"
#include "argument.hpp"


template<border_pos Border>
struct disable_border : argument<app_settings>
{
    argument::argtype type() const override
    {
        return argument::argtype::toggle;
    }

    std::string_view symbol() const override
    {
        if constexpr (Border == border_pos::top)
            return "T";
        else if constexpr (Border == border_pos::bot)
            return "B";
        else if constexpr (Border == border_pos::left)
            return "L";
        else if constexpr (Border == border_pos::right)
            return "R";
        else if constexpr (Border == border_pos::header)
            return "H";
        else if constexpr (Border == border_pos::del)
            return "D";
    }

    void operator()(app_settings& app) override
    {
        app.print.style.disable(Border);
    }
};


struct delimiter : argument<app_settings>
{
    argument::argtype type() const { return argument::argtype::next_arg_value; }

    std::string_view symbol() const { return "d"; }
    std::string_view string() const { return "delimiter"; }

    void operator()(app_settings& app, std::string_view val) override
    {
        app.parse.del = val;
    }
};
