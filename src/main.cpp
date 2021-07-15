#include <iostream>
#include <vector>
#include <string_view>

#include "include/table.hpp"
#include "include/unicode.hpp"
#include "include/printer.hpp"

#include "options.hpp"
#include "app_settings.hpp"


void perform_app(std::vector<std::string_view> args);

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
    // start with 1, skip program name
    for (int i = 1; i < argc; i++)
    {
        arg_vec.push_back(argv[i]);
    }
    return arg_vec;
}


void perform_app(std::vector<std::string_view> args)
{
    using namespace std::string_literals;

    set_locale_all("en_US.utf8");

    app_settings app{ std::cout };

    //
    // Setup arguments
    //
    app.options.push_back(std::make_unique<disable_top>());
    app.options.push_back(std::make_unique<test>());

    using opttype = decltype(app.options)::value_type::element_type::argtype;
    for (size_t i = 0; i < args.size(); i++)
    {
        for (auto& option : app.options)
        {
            if (args[i] == option->symbol()
                && option->type() == opttype::toggle)
            {
                (*option)(app);
            }
            else if (starts_with(args[i], option->string()))
            {
                switch (option->type())
                {
                    case opttype::enum_value:
                        args[i].remove_prefix(option->string().length());
                        (*option)(app, args[i]);
                        break;

                    case opttype::next_arg_value:
                        if (i == args.size())
                        {
                            throw std::runtime_error(
                                    "missing following parameter for option "s
                                    + std::string(option->string()));
                        }
                        (*option)(app, args[i + 1]);
                        i++;
                        break;

                    default:
                        break;
                }
            }
            else
            {
                throw std::runtime_error(
                        "invalid argument"s + std::string(args[i]));
            }
        }
    }

    table tab = table::parse(std::cin, app.parse);
    app.print.print(tab);
}
