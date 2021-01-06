
#include <iostream>
#include <string>
#include <array>
#include <type_traits>

namespace {

    constexpr std::size_t names_count = 6;

    template<typename CharT, typename TraitsT>
    struct severity_level_names {
        static const std::array<std::basic_string<CharT, TraitsT>, names_count> names;
    };

    template<typename CharT, typename TraitsT>
    const std::array<std::basic_string<CharT, TraitsT>, names_count> severity_level_names<CharT, TraitsT>::names = {
            "TRACE",
            "DEBUG",
            "INFO",
            "WARN",
            "ERROR",
            "FATAL"
    };
}
int main()
{
    auto& names = severity_level_names<char, std::char_traits<char>>::names;
    for (auto&& value : names) {
        std::cout << value << std::endl;
    }



    return 0;
}