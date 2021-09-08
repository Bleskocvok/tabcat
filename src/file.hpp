#pragma once

#include <stdexcept>
#include <fstream>

#include "app_settings.hpp"
#include "argument.hpp"


struct file : file_arg<app_settings>
{
    bool invoked = false;

    void operator()(app_settings& app, std::string filename) override
    {
        if (invoked)
        {
            throw std::runtime_error("program accepts only one file argument");
        }
        invoked = true;

        app.input = std::make_unique<file_in>(std::move(filename));
    }
};
