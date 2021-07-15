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
    for (int i = 0; i < argc; i++)
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
    app.options.push_back(std::make_unique<disable_border<border_pos::top>>());
    app.options.push_back(std::make_unique<disable_border<border_pos::bot>>());
    app.options.push_back(std::make_unique<disable_border<border_pos::left>>());
    app.options.push_back(std::make_unique<disable_border<border_pos::right>>());
    app.options.push_back(std::make_unique<disable_border<border_pos::header>>());
    app.options.push_back(std::make_unique<disable_border<border_pos::del>>());

    using opttype = decltype(app.options)::value_type::element_type::argtype;
    using s = std::string;
    // start with 1, skip program name
    for (size_t i = 1; i < args.size(); i++)
    {
        if (args[i].empty())
            continue;

        for (auto& option : app.options)
        {
            bool symbol = !option->symbol().empty()
                            && args[i] == "-"s + s(option->symbol());
            bool string = !option->string().empty()
                            && starts_with(args[i], "--"s + s(option->string()));

            if (string)
            {
                args[i].remove_prefix(option->string().length());
            }

            switch (option->type())
            {
                case opttype::toggle:
                    if (symbol || string)
                    {
                        (*option)(app);
                    }
                    break;

                case opttype::enum_value:
                    if (string)
                    {
                        args[i].remove_prefix("="s.length());
                        (*option)(app, args[i]);
                    }
                    break;

                case opttype::next_arg_value:
                    if (symbol || string)
                    {
                        if (i == args.size())
                        {
                            throw std::runtime_error(
                                    "missing following parameter for option "s
                                    + std::string(option->string()));
                        }
                        (*option)(app, args[i + 1]);
                        i++;
                    }
                    break;
            }
        }
    }

    table tab = table::parse(std::cin, app.parse);
    app.print.print(tab);
}
