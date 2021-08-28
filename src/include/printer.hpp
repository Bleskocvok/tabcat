#pragma once

#include <string_view>
#include <cassert>
#include <bitset>
#include <iomanip>      // setw
#include <functional>   // function

#include "table.hpp"
#include "terminal.hpp"


enum class border_pos : size_t
{
    top, bot, left, right, header, del,
    COUNT   // this MUST be last; contains the number of possible enum values
};


inline static constexpr auto border_name_str
        = std::array<std::string_view, static_cast<size_t>(border_pos::COUNT)>
            {
                "top", "bot", "left", "right", "header", "del"
            };


inline std::string to_str(border_pos val)
{
    assert(val < border_pos::COUNT);
    switch (val)
    {
        case border_pos::top: return "top";
        case border_pos::bot: return "bot";
        case border_pos::left: return "left";
        case border_pos::right: return "right";
        case border_pos::header: return "header";
        case border_pos::del: return "del";
        default:
            assert(false);
            return "";
            break;
    }
}


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

    constexpr
    print_style(border top,
                border mid,
                border bot,
                std::string_view del,
                std::string_view outer,
                std::string_view eol = "\n",
                std::string_view space = " ")
        : top(top)
        , mid(mid)
        , bot(bot)
        , del(del)
        , outer(outer)
        , eol(eol)
        , space(space) {}

    print_style() = default;

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

    void enable(border_pos b)
    {
        assert(b < border_pos::COUNT);
        disabled[static_cast<size_t>(b)] = 0;
    }
};


template<typename PrintOutput>
struct printer
{
    PrintOutput& out;
    print_style style;

    printer(PrintOutput& out)
        : out(out) {}

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
