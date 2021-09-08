#pragma once

#include <memory>       // unique_ptr
#include <vector>
#include <string>
#include <utility>      // forward
#include <ostream>
#include <optional>
#include <string_view>


enum class argtype
{
    toggle,         // on/off, takes no parameter
    enum_value,     // takes a parameter like this: --argname=<parameter>
    next_arg_value  // takes a parameter like this: --argname "<parameter>"
                    // or like this:                -a "<parameter>"
};


template<typename App>
struct argument
{
    virtual ~argument() = default;

    virtual argtype type() const { return argtype::toggle; }

    virtual std::optional<char>         symbol() const { return std::nullopt; }
    virtual std::optional<std::string>  string() const { return std::nullopt; }

    virtual std::string_view description() const { return ""; }

    virtual void operator()(App&) {}
    virtual void operator()(App&, std::string_view) {}
};


template<typename App>
struct file_arg
{
    virtual ~file_arg() = default;

    virtual void operator()(App&, std::string) {}
};


template<typename App>
class arguments
{

std::vector<std::unique_ptr<argument<App>>> opts;
std::unique_ptr<file_arg<App>> file_opt;

public:
    std::vector<std::unique_ptr<argument<App>>>& data() { return opts; }
    const std::vector<std::unique_ptr<argument<App>>>& data() const
    {
        return opts;
    }

    template<typename T, typename ... Args>
    void emplace(Args&& ... args)
    {
        opts.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    template<typename T, typename ... Args>
    void file_emplace(Args&& ... args)
    {
        file_opt = std::make_unique<T>(std::forward<Args>(args)...);
    }

    bool file_accepted() const { return file_opt.operator bool(); }

    [[nodiscard]]
    std::optional<std::string> perform(int argc, char** argv, App& app)
    {
        // TODO: this should probably be the main one
        return {};
    }

    [[nodiscard]]
    std::optional<std::string> perform(
            const std::vector<std::string_view>& args,
            App& app)
    {
        using namespace std::string_literals;
        using s = std::string;

        // start with 1, skip program name
        for (size_t i = 1; i < args.size(); i++)
        {
            if (args[i].empty())
                continue;

            size_t eq_idx = args[i].find('=');
            argument<App>* option = nullptr;

            if (starts_with(args[i], "--"))
            {
                auto name = args[i].substr(2, eq_idx - 2);
                option = opt_by_string(name);
            }
            else if (starts_with(args[i], "-"))
            {
                if (eq_idx != std::string_view::npos)
                {
                    return "invalid '='";
                }
                // case that options are coalesced, e.g.
                // ./executable -TBLR
                if (args[i].substr(1).length() > 1)
                {
                    for (char ch : args[i].substr(1))
                    {
                        option = opt_by_symbol(ch);
                        if (!option)
                            return "invalid argument '"s + ch + "'"s;
                        if (option->type() != argtype::toggle)
                            return "cannot coalesce argument '"s + ch + "'"s;
                        (*option)(app);
                    }
                    continue;
                }
                option = opt_by_symbol(args[i].at(1));
            }
            else if (file_accepted())
            {
                (*file_opt)(app, std::string(args[i]));
                continue;
            }

            if (!option)
            {
                return "invalid argument '"s + s(args[i]) + "'"s;
            }

            switch (option->type())
            {
                case argtype::toggle:
                    if (eq_idx != std::string_view::npos)
                    {
                        return "unexpected '='";
                    }
                    (*option)(app);
                    break;

                case argtype::enum_value:
                    if (eq_idx == std::string_view::npos)
                    {
                        return "expected '='";
                    }
                    (*option)(app, args[i].substr(eq_idx + 1));
                    break;

                case argtype::next_arg_value:
                    if (i == args.size() - 1)
                    {
                        return "expected parameter for argument '"s
                                + s(args[i]) + "'"s;
                    }
                    (*option)(app, args[i + 1]);
                    i++;
                    break;
            }
        }
        return std::nullopt;
    }

    template<typename Pred>
    argument<App>* find_opt(Pred pred)
    {
        auto it = std::find_if(opts.begin(), opts.end(), pred);

        if (it == opts.end())
            return nullptr;

        return it->get();
    }

    argument<App>* opt_by_symbol(char sym)
    {
        return find_opt([=](const auto& opt)
        {
            return opt->symbol() == sym;
        });
    }

    argument<App>* opt_by_string(std::string_view str)
    {
        return find_opt([=](const auto& opt)
        {
            return opt->string() == str;
        });
    }

    void print_help(std::ostream& out) const
    {
        using namespace std::string_literals;

        for (const auto& opt : opts)
        {
            out << "  ";

            std::string symbol = "  ";
            symbol += !opt->symbol() ? ""
                                     : "-"s + *opt->symbol();
            if (opt->symbol() && opt->string())
                symbol += ",";

            std::string str;
            str += !opt->string() ? ""
                                  : "--"s + *opt->string();

            if (opt->type() == argtype::enum_value)
            {
                str += "=PARAM";
            }

            if (opt->type() == argtype::next_arg_value)
            {
                str += " PARAM";
            }

            out << std::left
                << std::setw(6) << symbol
                << std::setw(22) << str << " "
                << opt->description() << "\n";
        }
    }
};
