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

    std::string_view description() const override
    {
        if constexpr (Border == border_pos::top)
            return "disable TOP border";
        else if constexpr (Border == border_pos::bot)
            return "disable BOTTOM border";
        else if constexpr (Border == border_pos::left)
            return "disable LEFT border";
        else if constexpr (Border == border_pos::right)
            return "disable RIGHT border";
        else if constexpr (Border == border_pos::header)
            return "disable border below HEADER";
        else if constexpr (Border == border_pos::del)
            return "disable DELIMITER between columns";
    }

    void operator()(app_settings& app) override
    {
        app.print.style.disable(Border);
    }
};
