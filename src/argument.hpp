#pragma once

#include <memory>       // unique_ptr
#include <vector>
#include <string>
#include <optional>
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


template<typename App>
using option_vec = std::vector<std::unique_ptr<argument<App>>>;


template<typename App, typename Pred>
argument<App>* find_opt(const option_vec<App>& options, Pred pred)
{
    auto it = std::find_if(options.begin(), options.end(), pred);

    if (it == options.end())
        return nullptr;

    return it->get();
}


template<typename App>
argument<App>* opt_by_symbol(const option_vec<App>& opts,
                             std::string_view sym)
{
    if (sym.empty())
        return nullptr;

    return find_opt(opts, [=](const auto& opt){ return opt->symbol() == sym; });
}


template<typename App>
argument<App>* opt_by_string(const option_vec<App>& opts,
                             std::string_view str)
{
    if (str.empty())
        return nullptr;

    return find_opt(opts, [=](const auto& opt){ return opt->string() == str; });
}


template<typename App>
[[nodiscard]]
inline std::optional<std::string> perform_args(
        const std::vector<std::string_view>& args,
        App& app,
        const option_vec<App>& options)
{
    using namespace std::string_literals;
    using s = std::string;
    using opttype = typename argument<App>::argtype;

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
            option = opt_by_string(options, name);
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
                    auto view = std::string_view(&ch);
                    option = opt_by_symbol(options, view);
                    if (!option)
                        return "invalid argument '"s + s(view) + "'"s;
                    if (option->type() != opttype::toggle)
                        return "cannot coalesce argument '"s + s(view) + "'"s;
                    (*option)(app);
                }
                continue;
            }
            option = opt_by_symbol(options, args[i].substr(1));
        }
        else
        {
            return "invalid argument '"s + s(args[i]) + "'"s;
        }

        if (!option)
        {
            return "invalid argument '"s + s(args[i]) + "'"s;
        }

        switch (option->type())
        {
            case opttype::toggle:
                if (eq_idx != std::string_view::npos)
                {
                    return "unexpected '='";
                }
                (*option)(app);
                break;

            case opttype::enum_value:
                if (eq_idx == std::string_view::npos)
                {
                    return "expected '='";
                }
                (*option)(app, args[i].substr(eq_idx + 1));
                break;

            case opttype::next_arg_value:
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
