#pragma once

#include <string>

#include "app_settings.hpp"
#include "argument.hpp"
#include "io.hpp"


struct output_file : argument<app_settings>
{
    argtype type() const override
    {
        return argtype::next_arg_value;
    }

    std::optional<char> symbol() const override { return 'o'; }

    std::string_view description() const override
    {
        return "set an output file";
    }

    void operator()(app_settings& app, std::string_view val) override
    {
        app.output = std::make_unique<file_out>(std::string(val));
    }
};
