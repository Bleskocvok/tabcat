#pragma once

#include "app_settings.hpp"
#include "argument.hpp"


struct locale : argument<app_settings>
{
    argtype type() const override
    {
        return argtype::enum_value;
    }

    std::optional<std::string> string() const override { return "locale"; }

    std::string_view description() const override
    {
        return "set the current locale";
    }

    void operator()(app_settings&, std::string_view val) override
    {
        set_locale_all(std::string(val));
    }
};
