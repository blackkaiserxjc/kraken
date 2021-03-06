#pragma once

#include <kr/io/detail/bind_handler.h>

#include <type_traits>
#include <utility>

namespace kr {
namespace network {

template <class Handler, class... Args>
detail::bind_wrapper<
    typename std::decay<Handler>::type,
    typename std::decay<Args>::type...>
bind_handler(Handler &&handler, Args &&...args)
{
    return detail::bind_wrapper<
        typename std::decay<Handler>::type,
        typename std::decay<Args>::type...>(
        std::forward<Handler>(handler),
        std::forward<Args>(args)...);
}

} // namespace network
} // namespace kr
