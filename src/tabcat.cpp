#include <iostream>
#include <vector>
#include <string_view>

#include "include/table.hpp"
#include "include/unicode.hpp"
#include "include/printer.hpp"

#include "app_settings.hpp"

// argument includes
#include "delimiter.hpp"
#include "disable_border.hpp"
#include "help.hpp"
#include "version.hpp"
#include "format.hpp"
#include "locale.hpp"


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
        return 2;
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
    using namespace std::string_literals;

    assert(args.size() >= 1);

    set_locale_all("en_US.utf8");

    app_settings app{ std::cout };
    app.program_name = args[0];

    // parsing
    app.options.push_back(std::make_unique<delimiter>());
    app.options.push_back(std::make_unique<locale>());
    // output style
    app.options.push_back(std::make_unique<format>());
    // borders disabling
    app.options.push_back(std::make_unique<disable_border<border_pos::top>>());
    app.options.push_back(std::make_unique<disable_border<border_pos::bot>>());
    app.options.push_back(std::make_unique<disable_border<border_pos::left>>());
    app.options.push_back(std::make_unique<disable_border<border_pos::right>>());
    app.options.push_back(std::make_unique<disable_border<border_pos::header>>());
    app.options.push_back(std::make_unique<disable_border<border_pos::del>>());
    // other
    app.options.push_back(std::make_unique<version>());
    app.options.push_back(std::make_unique<help>());

    auto error = perform_args(args, app, app.options);
    if (error)
        throw std::runtime_error(*error);

    if (app.state != app_state::cont)
        return;

    table tab = table::parse(std::cin, app.parse);
    app.print.print(tab);
}