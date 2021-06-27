//
// Created by kaiser on 2021/3/21.
//

#include "string.h"

#include <cassert>
#include <sstream>

#include <fmt/format.h>

#include <kr/core/core.h>

namespace kr {
namespace utility {

namespace detail {

struct string_table_hex_make_item
{
    constexpr unsigned char operator()(std::size_t index) const
    {
        // clang-format off
    return static_cast<unsigned char>(
        index >= '0' && index <= '9' ? index - '0' :
        index >= 'a' && index <= 'f' ? index - 'a' + 10 :
        index >= 'A' && index <= 'F' ? index - 'A' + 10 :
        16);
        // clang-format on
    }
};

struct string_table_uri_escape_make_item
{
    constexpr unsigned char operator()(std::size_t index) const
    {
        // clang-format off
    return static_cast<unsigned char>(
        index >= '0' && index <= '9' ? 0 :
        index >= 'A' && index <= 'Z' ? 0 :
        index >= 'a' && index <= 'z' ? 0 :
        index == '-' ? 0 :
        index == '_' ? 0 :
        index == '.' ? 0 :
        index == '~' ? 0 :
        index == '/' ? 2 :
        index == ' ' ? 3 :
        4);
        // clang-format on
    }
};

template <typename MakeItem, std::size_t... Index>
constexpr auto make_array_helper(
    MakeItem const &make, std::index_sequence<Index...>)
{
    return std::array<decltype(make(0)), sizeof...(Index)>{{make(Index)...}};
}

template <std::size_t Size, typename MakeItem>
constexpr auto make_array_with(MakeItem const &make)
{
    return make_array_helper(make, std::make_index_sequence<Size>{});
}

//extern const std::array<unsigned char, 256> hex_table;
//extern const std::array<unsigned char, 256> uri_escape_table;

static constexpr std::array<unsigned char, 256> hex_table =
    make_array_with<256>(string_table_hex_make_item{});

static constexpr std::array<unsigned char, 256> uri_escape_table =
    make_array_with<256>(string_table_uri_escape_make_item{});
} // namespace detail

std::string uri_escape(std::string_view str, UriEscapeMode mode)
{
    std::string result;
    static const char hex_values[] = "0123456789abcdef";
    char esc[3] = {0};
    esc[0] = '%';

    result.reserve(result.size() + str.size() + 3 * (str.size()) / 4);
    auto p = str.begin();
    auto last = p;

    auto min_encode = static_cast<std::uint8_t>(mode);
    while (p != str.end())
    {
        char c = *p;
        unsigned char v = static_cast<unsigned char>(c);
        unsigned char discriminator = detail::uri_escape_table[v];
        if (LIKELY(discriminator <= min_encode))
        {
            ++p;
        }
        else if (mode == UriEscapeMode::QUERY && discriminator == 3)
        {
            result.append(&*last, size_t(p - last));
            result.push_back('+');
            ++p;
            last = p;
        }
        else
        {
            result.append(&*last, size_t(p - last));
            esc[1] = hex_values[v >> 4];
            esc[2] = hex_values[v & 0x0f];
            result.append(esc, 3);
            ++p;
            last = p;
        }
    }
    result.append(&*last, size_t(p - last));
    return result;
}

std::string uri_unescape(std::string_view str, UriEscapeMode mode)
{
    std::string result;
    result.reserve(result.size() + str.size());
    auto p = str.begin();
    auto last = p;

    while (p != str.end())
    {
        char c = *p;
        switch (c)
        {
        case '%': {
            if (UNLIKELY(std::distance(p, str.end()) < 3))
            {
                throw std::invalid_argument("incomplete percent encode sequence");
            }
            auto h1 = detail::hex_table[static_cast<unsigned char>(p[1])];
            auto h2 = detail::hex_table[static_cast<unsigned char>(p[2])];
            if (UNLIKELY(h1 == 16 || h2 == 16))
            {
                throw std::invalid_argument("invalid percent encode sequence");
            }
            result.append(&*last, size_t(p - last));
            result.push_back((h1 << 4) | h2);
            p += 3;
            last = p;
            break;
        }
        case '+':
            if (mode == UriEscapeMode::QUERY)
            {
                result.append(&*last, size_t(p - last));
                result.push_back(' ');
                ++p;
                last = p;
                break;
            }
            [[fallthrough]];
        default:
            ++p;
            break;
        }
    }
    result.append(&*last, size_t(p - last));
    return result;
}
namespace {
struct PrettySuffix
{
    const char *suffix;
    double val;
};

const PrettySuffix kPrettyTimeSuffixes[] = {
    {"s ", 1e0L},
    {"ms", 1e-3L},
    {"us", 1e-6L},
    {"ns", 1e-9L},
    {"ps", 1e-12L},
    {"s ", 0},
    {nullptr, 0},
};

const PrettySuffix kPrettyTimeHmsSuffixes[] = {
    {"h ", 60L * 60L},
    {"m ", 60L},
    {"s ", 1e0L},
    {"ms", 1e-3L},
    {"us", 1e-6L},
    {"ns", 1e-9L},
    {"ps", 1e-12L},
    {"s ", 0},
    {nullptr, 0},
};

const PrettySuffix kPrettyBytesMetricSuffixes[] = {
    {"EB", 1e18L},
    {"PB", 1e15L},
    {"TB", 1e12L},
    {"GB", 1e9L},
    {"MB", 1e6L},
    {"kB", 1e3L},
    {"B ", 0L},
    {nullptr, 0},
};

const PrettySuffix kPrettyBytesBinarySuffixes[] = {
    {"EB", int64_t(1) << 60},
    {"PB", int64_t(1) << 50},
    {"TB", int64_t(1) << 40},
    {"GB", int64_t(1) << 30},
    {"MB", int64_t(1) << 20},
    {"kB", int64_t(1) << 10},
    {"B ", 0L},
    {nullptr, 0},
};

const PrettySuffix kPrettyBytesBinaryIECSuffixes[] = {
    {"EiB", int64_t(1) << 60},
    {"PiB", int64_t(1) << 50},
    {"TiB", int64_t(1) << 40},
    {"GiB", int64_t(1) << 30},
    {"MiB", int64_t(1) << 20},
    {"KiB", int64_t(1) << 10},
    {"B  ", 0L},
    {nullptr, 0},
};

const PrettySuffix kPrettyUnitsMetricSuffixes[] = {
    {"qntl", 1e18L},
    {"qdrl", 1e15L},
    {"tril", 1e12L},
    {"bil", 1e9L},
    {"M", 1e6L},
    {"k", 1e3L},
    {" ", 0},
    {nullptr, 0},
};

const PrettySuffix kPrettyUnitsBinarySuffixes[] = {
    {"E", int64_t(1) << 60},
    {"P", int64_t(1) << 50},
    {"T", int64_t(1) << 40},
    {"G", int64_t(1) << 30},
    {"M", int64_t(1) << 20},
    {"k", int64_t(1) << 10},
    {" ", 0},
    {nullptr, 0},
};

const PrettySuffix kPrettyUnitsBinaryIECSuffixes[] = {
    {"Ei", int64_t(1) << 60},
    {"Pi", int64_t(1) << 50},
    {"Ti", int64_t(1) << 40},
    {"Gi", int64_t(1) << 30},
    {"Mi", int64_t(1) << 20},
    {"Ki", int64_t(1) << 10},
    {"  ", 0},
    {nullptr, 0},
};

const PrettySuffix kPrettySISuffixes[] = {
    {"Y", 1e24L},
    {"Z", 1e21L},
    {"E", 1e18L},
    {"P", 1e15L},
    {"T", 1e12L},
    {"G", 1e9L},
    {"M", 1e6L},
    {"k", 1e3L},
    {"h", 1e2L},
    {"da", 1e1L},
    {"d", 1e-1L},
    {"c", 1e-2L},
    {"m", 1e-3L},
    {"u", 1e-6L},
    {"n", 1e-9L},
    {"p", 1e-12L},
    {"f", 1e-15L},
    {"a", 1e-18L},
    {"z", 1e-21L},
    {"y", 1e-24L},
    {" ", 0},
    {nullptr, 0},
};

const PrettySuffix *const kPrettySuffixes[PRETTY_NUM_TYPES] = {
    kPrettyTimeSuffixes,
    kPrettyTimeHmsSuffixes,
    kPrettyBytesMetricSuffixes,
    kPrettyBytesBinarySuffixes,
    kPrettyBytesBinaryIECSuffixes,
    kPrettyUnitsMetricSuffixes,
    kPrettyUnitsBinarySuffixes,
    kPrettyUnitsBinaryIECSuffixes,
    kPrettySISuffixes,
};

} // namespace

std::string pretty_print(double val, PrettyType type, bool add_space)
{
    assert(type >= 0 && type < PRETTY_NUM_TYPES);
    auto suffixes = kPrettySuffixes[type];

    double abs_val = fabs(val);
    for (auto i = 0; suffixes[i].suffix; ++i)
    {
        if (abs_val >= suffixes[i].val)
        {
            return fmt::format("{:.4g}{}{}",
                (suffixes[i].val ? (val / suffixes[i].val) : val),
                (add_space ? " " : ""),
                suffixes[i].suffix);
        }
    }
    return fmt::format("{:.4g}", val);
}

std::string hex_dump(const void *ptr, std::size_t size)
{
    std::ostringstream os;
    hex_dump(ptr, size, std::ostream_iterator<std::string_view>(os, "\n"));
    return os.str();
}

namespace detail {

std::size_t hex_dump_line(const void *ptr, std::size_t offset, std::size_t size, std::string &line)
{
    static char hex_values[] = "0123456789abcdef";
    line.clear();
    const uint8_t *p = reinterpret_cast<const uint8_t *>(ptr) + offset;
    size_t n = std::min(size - offset, size_t(16));
    line.push_back(hex_values[(offset >> 28) & 0xf]);
    line.push_back(hex_values[(offset >> 24) & 0xf]);
    line.push_back(hex_values[(offset >> 20) & 0xf]);
    line.push_back(hex_values[(offset >> 16) & 0xf]);
    line.push_back(hex_values[(offset >> 12) & 0xf]);
    line.push_back(hex_values[(offset >> 8) & 0xf]);
    line.push_back(hex_values[(offset >> 4) & 0xf]);
    line.push_back(hex_values[offset & 0xf]);
    line.push_back(' ');

    for (size_t i = 0; i < n; i++)
    {
        if (i == 8)
        {
            line.push_back(' ');
        }

        line.push_back(' ');
        line.push_back(hex_values[(p[i] >> 4) & 0xf]);
        line.push_back(hex_values[p[i] & 0xf]);
    }
    line.append(3 * (16 - n) + (n <= 8), ' ');
    line.append("  |");

    for (size_t i = 0; i < n; i++)
    {
        char c = (p[i] >= 32 && p[i] <= 126 ? static_cast<char>(p[i]) : '.');
        line.push_back(c);
    }
    line.append(16 - n, ' ');
    line.push_back('|');
    assert(line.size() == 78u);
    return n;
}
} // namespace detail
} // namespace utility
} // namespace kr
