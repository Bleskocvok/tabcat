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
#include "file.hpp"
#include "output_file.hpp"


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

    app_settings app;
    app.program_name = args[0];


    // parsing
    app.args.emplace<delimiter>();
    app.args.emplace<locale>();
    // output style
    app.args.emplace<format>();
    app.args.emplace<output_file>();
    // borders disabling
    app.args.emplace<disable_border<border_pos::top>>();
    app.args.emplace<disable_border<border_pos::bot>>();
    app.args.emplace<disable_border<border_pos::left>>();
    app.args.emplace<disable_border<border_pos::right>>();
    app.args.emplace<disable_border<border_pos::header>>();
    app.args.emplace<disable_border<border_pos::del>>();
    // other
    app.args.emplace<version>();
    app.args.emplace<help>();
    // file
    app.args.file_emplace<file>();


    auto error = app.args.perform(args, app);
    if (error)
        throw std::runtime_error(*error);

    if (app.state != app_state::cont)
        return;

    app.input->open();
    table tab = table::parse(app.input->istream(), app.parse);
    app.input->close();

    app.output->open();
    app.print.output(app.output->ostream());
    app.print.print(tab);
    app.output->close();
}
