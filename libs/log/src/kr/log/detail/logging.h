#ifndef KR_COMMON_LOGGING_DETAIL_LOGGER_H_
#define KR_COMMON_LOGGING_DETAIL_LOGGER_H_

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

#include <boost/algorithm/string.hpp>

#include <kr/log/logging.h>

namespace kr {
namespace log {

namespace {
constexpr std::size_t names_count = 6;
} // namespace

template <typename CharT, typename TraitsT>
std::basic_string<CharT, TraitsT> to_string(severity_level level)
{
    static std::array<std::string, names_count> level_names = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
    if (static_cast<std::size_t>(level) < names_count)
    {
        return level_names[static_cast<std::size_t>(level)];
    }
    return std::to_string(static_cast<std::size_t>(level));
}

template <typename CharT, typename TraitsT>
bool from_string(const std::basic_string<CharT, TraitsT> &str, severity_level &level)
{
    static std::array<std::string, names_count> level_names = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
    auto iter = std::find_if(level_names.begin(), level_names.end(), [&](auto &&value) {
        return std::equal(str.begin(), str.end(), value.begin(), value.end());
    });

    if (iter != level_names.end())
    {
        level = static_cast<severity_level>(iter - level_names.begin());
        return true;
    }
    return false;
}

template <typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT> &operator<<(std::basic_ostream<CharT, TraitsT> &strm, severity_level level)
{
    strm << to_string<CharT, TraitsT>(level);
    return strm;
}

template <typename CharT, typename TraitsT>
std::basic_istream<CharT, TraitsT> &operator>>(std::basic_istream<CharT, TraitsT> &strm, severity_level &level)
{
    using string_type = std::basic_string<CharT, TraitsT>;
    string_type str;
    strm >> str;
    from_string(str, level);
    return strm;
}

template <typename ValueType>
ValueType logger::set_attr_value(const char *name, ValueType value)
{
    using value_type = boost::log::attributes::mutable_constant<ValueType>;
    auto attr = boost::log::attribute_cast<value_type>(get_attributes()[name]);
    attr.set(value);
    return attr.get();
}

template <typename ValueType>
ValueType logger_mt::set_attr_value(const char *name, ValueType value)
{
    using value_type = boost::log::attributes::mutable_constant<ValueType>;
    auto attr = boost::log::attribute_cast<value_type>(this->get_attributes()[name]);
    attr.set(value);
    return attr.get();
}

} // namespace log
} // namespace kr

#endif //KR_COMMON_LOGGING_DETAIL_LOGGER_H_