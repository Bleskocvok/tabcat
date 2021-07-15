#pragma once

#include <string_view>


template<typename App>
struct argument
{
    enum class argtype
    {
        toggle,         // on/off, takes no parameter
        enum_value,     // takes a parameter like this: --argname=<parameter>
        next_arg_value  // takes a parameter like this: --argname "<parameter>"
                        // or like this:                -a "<parameter>"
    };

    virtual ~argument() = default;

    virtual argtype type() const { return argtype::toggle; }

    virtual std::string_view symbol() const { return ""; }
    virtual std::string_view string() const { return ""; }

    virtual std::string_view description() const { return ""; }

    virtual void operator()(App&) {}
    virtual void operator()(App&, std::string_view) {}
};
