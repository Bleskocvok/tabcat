#pragma once

#include <string_view>

#include "table.hpp"


enum class border_pos
{
    top, bot, left, right, header, del,
    COUNT   // this MUST be last; contains the number of possible enum values
};


struct print_style
{
    struct border
    {
        std::string_view beg = "",
                         mid = "",
                         join = "",
                         end = "";
    };

    border top{ "┌", "─", "┬", "┐" };
    border mid{ "├", "─", "┼", "┤" };
    border bot{ "└", "─", "┴", "┘" };

    std::string_view del    = "│",
                     outer  = "│";

    std::string_view eol = "\n",
                     space = " ";
    size_t spacing = 1;

    struct
    {
        bool top    = false,
             bot    = false,
             left   = false,
             right  = false,
             del    = false,
             header = false;
    }
    disable;
};


template<typename PrintOutput>
struct printer
{
    PrintOutput& out;

    printer(PrintOutput& out)
        : out(out) {}

    print_style style;

    template<typename T>
    void repeat(const T& t, size_t count) const
    {
        for (size_t i = 0; i < count; i++)
        {
            out << t;
        }
    }

    void print_header(const table& tab
                    , const print_style::border& bstyle
                    , std::string_view eol) const
    {
        out << bstyle.beg << bstyle.mid;
        bool prefix = false;
        for (size_t i = 0; i < tab.column_count(); i++)
        {
            size_t width = tab.columns[i].width;

            if (prefix)
                out << bstyle.mid << bstyle.join << bstyle.mid;
            else
                prefix = true;

            for (size_t i = 0; i < width; i++)
            {
                out << bstyle.mid;
            }
        }
        out << bstyle.mid << bstyle.end << eol;
    }

    void print(const table& tab) const
    {
        top_border(tab);

        auto print_cell = [&](auto str, location loc)
        {
            if (loc.first_col)
            {
                left_border();
            }
            else
            {
                delimiter();
            }

            out << (is_numeric(str) ? std::right : std::left)
                << std::setw(tab.columns[loc.col].width - size_diff(str))
                << str;

            if (loc.last_col)
            {
                right_border();
                out << style.eol;
            }

            if (loc.row == 0 && loc.last_col)
                header_border(tab);
        };
        tab.for_each_cell(print_cell);

        bot_border(tab);
    }

    void spacing() const
    {
        repeat(style.space, style.spacing);
    }

    // TODO: refactor *_border(...) functions using enums

    void top_border(const table& tab) const
    {
        if (style.disable.top)
            return;

        print_header(tab, style.top, style.eol);
    }

    void bot_border(const table& tab) const
    {
        if (style.disable.bot)
            return;

        print_header(tab, style.bot, style.eol);
    }

    void header_border(const table& tab) const
    {
        if (style.disable.header)
            return;

        print_header(tab, style.mid, style.eol);
    }

    void left_border() const
    {
        if (style.disable.left)
            return;

        out << style.outer;
        spacing();
    }

    void delimiter() const
    {
        spacing();
        if (style.disable.del)
            return;
        out << style.del;
        spacing();
    }

    void right_border() const
    {
        if (style.disable.right)
            return;

        spacing();
        out << style.outer;
    }
};
