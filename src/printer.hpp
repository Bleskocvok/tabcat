#pragma once

#include <string_view>
#include <cassert>
#include <bitset>

#include "table.hpp"
#include "terminal.hpp"


enum class border_pos : size_t
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

    std::bitset<static_cast<size_t>(border_pos::COUNT)> disabled{ 0 };

    bool is_disabled(border_pos b) const
    {
        assert(b < border_pos::COUNT);
        return disabled[static_cast<size_t>(b)];
    }

    void disable(border_pos b)
    {
        assert(b < border_pos::COUNT);
        disabled[static_cast<size_t>(b)] = 1;
    }
};


template<typename PrintOutput>
struct printer
{
    PrintOutput& out;
    print_style style;

    printer(PrintOutput& out)
        : out(out)
    {
        // TODO: remove this; it's for debugging
        style.disable(border_pos::top);
        style.disable(border_pos::bot);
        // style.disable(border_pos::left);
        // style.disable(border_pos::right);
        // style.disable(border_pos::header);
        // style.disable(border_pos::del);
    }

    template<typename T>
    void repeat(const T& t, size_t count) const
    {
        for (size_t i = 0; i < count; i++)
        {
            out << t;
        }
    }

    void print_header(const table& tab
                    , const print_style::border& bstyle) const
    {
        if (!style.is_disabled(border_pos::left))
        {
            out << bstyle.beg;
            repeat(bstyle.mid, style.spacing);
        }

        bool prefix = false;
        for (size_t i = 0; i < tab.column_count(); i++)
        {
            size_t width = tab.columns[i].width;

            if (!prefix)
            {
                prefix = true;
            }
            else if (style.is_disabled(border_pos::del))
            {
                repeat(bstyle.mid, style.spacing);
            }
            else
            {
                repeat(bstyle.mid, style.spacing);
                out << bstyle.join;
                repeat(bstyle.mid, style.spacing);
            }

            repeat(bstyle.mid, width);
        }

        if (!style.is_disabled(border_pos::right))
        {
            repeat(bstyle.mid, style.spacing);
            out << bstyle.end;
        }
        out << style.eol;
    }

    void print(const table& tab) const
    {
        print(border_pos::top, tab);

        auto print_cell = [&](auto str, location loc)
        {
            if (loc.row % 2 == 0)
            {
                out << color::black;
                out << "\033[48;2;232;194;882m";
            }
            else
            {
                out << color::black;
                out << "\033[48;2;168;137;502m";
            }

            if (loc.first_col)
            {
                print(border_pos::left, tab);
            }
            else
            {
                spacing();
                print(border_pos::del, tab);
            }

            out << (is_numeric(str) ? std::right : std::left)
                << std::setw(tab.columns[loc.col].width - size_diff(str))
                << str;

            if (loc.last_col)
            {
                print(border_pos::right, tab);
                out << style.eol;
            }

            if (loc.row == 0 && loc.last_col)
            {
                print(border_pos::header, tab);
            }

            out << color::reset;
        };
        tab.for_each_cell(print_cell);

        print(border_pos::bot, tab);
    }

    void spacing() const
    {
        repeat(style.space, style.spacing);
    }

    void print(border_pos border, const table& tab) const
    {
        using b = border_pos;

        assert(border < b::COUNT);

        if (style.is_disabled(border))
            return;

        switch (border)
        {
            case b::top:
                print_header(tab, style.top);
                break;

            case b::bot:
                print_header(tab, style.bot);
                break;

            case b::left:
                out << style.outer;
                spacing();
                break;

            case b::right:
                spacing();
                out << style.outer;
                break;

            case b::header:
                print_header(tab, style.mid);
                break;

            case b::del:
                out << style.del;
                spacing();
                break;

            default:
                assert(false);
                break;
        }
    }
};
