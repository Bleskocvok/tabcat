#include <iostream>
#include <vector>
#include <string_view>

#include "include/table.hpp"
#include "include/unicode.hpp"
#include "include/printer.hpp"

#include "app_settings.hpp"

// arguments
#include "delimiter.hpp"
#include "disable_border.hpp"
#include "help.hpp"


void perform_app(const std::vector<std::string_view>& args);

std::vector<std::string_view> get_arg_vec(int argc, char** argv);


int main(int argc, char** argv)
{
    auto arg_vec = get_arg_vec(argc, argv);

    try
    {
        perform_app(arg_vec);
    }
    catch (invalid_table& ex)
    {
        std::cerr << "Invalid table: " << ex.what() << std::endl;
        return 1;
    }
    catch (std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}


std::vector<std::string_view> get_arg_vec(int argc, char** argv)
{
    std::vector<std::string_view> arg_vec;
    arg_vec.reserve(argc);
    for (int i = 0; i < argc; i++)
    {
        arg_vec.push_back(argv[i]);
    }
    return arg_vec;
}


void perform_app(const std::vector<std::string_view>& args)
{
    assert(args.size() >= 1);

    using namespace std::string_literals;

    set_locale_all("en_US.utf8");

    app_settings app{ std::cout };
    app.program_name = args[0];

    //
    // Setup arguments
    //
    // parsing
    app.options.push_back(std::make_unique<delimiter>());
    // borders disabling
    app.options.push_back(std::make_unique<disable_border<border_pos::top>>());
    app.options.push_back(std::make_unique<disable_border<border_pos::bot>>());
    app.options.push_back(std::make_unique<disable_border<border_pos::left>>());
    app.options.push_back(std::make_unique<disable_border<border_pos::right>>());
    app.options.push_back(std::make_unique<disable_border<border_pos::header>>());
    app.options.push_back(std::make_unique<disable_border<border_pos::del>>());
    // other
    app.options.push_back(std::make_unique<help>());

    using opttype = decltype(app.options)::value_type::element_type::argtype;
    // start with 1, skip program name
    for (size_t i = 1; i < args.size(); i++)
    {
        if (args[i].empty())
            continue;

        size_t eq_idx = args[i].find('=');
        argument<app_settings>* option = nullptr;

        if (starts_with(args[i], "--"))
        {
            auto name = args[i].substr(2, eq_idx - 2);
            option = app.opt_by_string(name);
        }
        else if (starts_with(args[i], "-"))
        {
            if (eq_idx != std::string_view::npos)
            {
                // TODO: error, invalid =
                throw std::runtime_error("error");
            }
            option = app.opt_by_symbol(args[i].substr(1));
        }
        else
        {
            // TODO: error
            throw std::runtime_error("invalid argument");
        }

        if (!option)
        {
            // TODO: error
            throw std::runtime_error("not found");
        }

        switch (option->type())
        {
            case opttype::toggle:
                if (eq_idx != std::string_view::npos)
                {
                    // TODO: error
                    throw std::runtime_error("error");
                }
                (*option)(app);
                break;

            case opttype::enum_value:
                if (eq_idx == std::string_view::npos)
                {
                    // TODO: error
                    throw std::runtime_error("error");
                }
                (*option)(app, args[i].substr(eq_idx + 1));
                break;

            case opttype::next_arg_value:
                if (i == args.size() - 1)
                {
                    // TODO: error
                    throw std::runtime_error("error");
                }
                (*option)(app, args[i + 1]);
                i++;
                break;
        }
    }

    if (app.state != app_state::cont)
        return;

    table tab = table::parse(std::cin, app.parse);
    app.print.print(tab);
}
