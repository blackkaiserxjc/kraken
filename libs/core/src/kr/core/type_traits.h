#pragma once

#include <string>
#include <string_view>
#include <type_traits>

namespace kr {
namespace core {

template <typename T>
using is_string = typename std::disjunction<
    std::is_same<char *, typename std::decay_t<T>>,
    std::is_same<const char *, typename std::decay_t<T>>,
    std::is_same<std::string, std::decay_t<T>>,
    std::is_same<std::string_view, std::decay_t<T>>>;

template <typename T>
inline constexpr bool is_string_v = is_string<T>::value;

} // namespace core
} // namespace kr