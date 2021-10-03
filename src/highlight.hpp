#pragma once

#include "include/colorize.hpp"
#include "app_settings.hpp"
#include "argument.hpp"


struct highlight : argument<app_settings>
{
    static constexpr char delimiter = ':';

    argtype type() const override
    {
        return argtype::next_arg_value;
    }

    std::optional<char> symbol() const override { return 'c'; }

    std::optional<std::string> string() const override { return "colorize"; }

    std::string_view description() const override
    {
        return "print specific strings highlighted with given colors [STRING:COLOR]";
    }

    void operator()(app_settings& app, std::string_view val) override
    {
        auto& highlights = app.print.highlights;
        if (!highlights)
            highlights.emplace();

        constexpr auto npos = std::string_view::npos;
        size_t split = 0;
        if ((split = val.rfind(delimiter)) == npos)
        {
            throw std::runtime_error("expected `:` between string and color");
        }

        auto string = val.substr(0, split);
        auto color = val.substr(split + 1);

        auto str_color = [](auto str)
        {
            if (str == "black")
                return fg_color::black;
            if (str == "red")
                return fg_color::red;
            if (str == "green")
                return fg_color::green;
            if (str == "yellow")
                return fg_color::yellow;
            if (str == "blue")
                return fg_color::blue;
            if (str == "magenta")
                return fg_color::magenta;
            if (str == "cyan")
                return fg_color::cyan;
            if (str == "white")
                return fg_color::white;
            throw std::runtime_error("invalid color");
        };

        highlights->emplace(string, str_color(color));
    }
};
