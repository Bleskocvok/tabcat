#pragma once

#include <stdexcept>

#include "app_settings.hpp"
#include "argument.hpp"


inline static constexpr print_style ascii_style
{
    print_style::border{ "+", "-", "+", "+" },
    print_style::border{ "+", "-", "+", "+" },
    print_style::border{ "+", "-", "+", "+" },
    "|",
    "|"
};
inline static constexpr print_style unicode_style
{
    print_style::border{ "┌", "─", "┬", "┐" },
    print_style::border{ "├", "─", "┼", "┤" },
    print_style::border{ "└", "─", "┴", "┘" },
    "│",
    "│"
};
inline static constexpr print_style tex_style
{
    print_style::border{ },
    print_style::border{ },
    print_style::border{ },
    "&",
    "",
    "\\\\\n"
};
inline static constexpr print_style md_style
{
    print_style::border{ "|", " ", " ", "|" },
    print_style::border{ "|", "-", "|", "|" },
    print_style::border{ "|", " ", " ", "|" },
    "|",
    "|"
};


struct format : argument<app_settings>
{
    argtype type() const override
    {
        return argtype::enum_value;
    }

    std::optional<std::string> string() const override { return "format"; }

    std::string_view description() const override
    {
        return "set output format [ascii/unicode/tex/md]";
    }

    void operator()(app_settings& app, std::string_view val) override
    {
        auto disabled = app.print.style.disabled;

        if (val == "ascii")
        {
            app.print.style = ascii_style;
            app.print.style.disabled = disabled;
        }
        else if (val == "unicode")
        {
            app.print.style = unicode_style;
            app.print.style.disabled = disabled;
        }
        else if (val == "tex")
        {
            app.print.style = tex_style;
            app.print.style.disabled = disabled;
            app.print.style.disable(border_pos::top);
            app.print.style.disable(border_pos::bot);
            app.print.style.disable(border_pos::left);
            app.print.style.disable(border_pos::right);
            app.print.style.disable(border_pos::header);
        }
        else if (val == "md")
        {
            app.print.style = md_style;
            app.print.style.disabled = disabled;
            app.print.style.disable(border_pos::top);
            app.print.style.disable(border_pos::bot);
        }
        else
        {
            using namespace std::string_literals;
            throw std::runtime_error("invalid format value '"s
                                     + std::string(val) + "'"s);
        }
    }
};
