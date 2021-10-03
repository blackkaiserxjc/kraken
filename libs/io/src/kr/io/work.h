#pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <kr/io/detail/work.h>

namespace kr {
namespace network {

template <typename Handler>
auto dispatch(Handler &&handler)
    -> detail::work_dispatcher<std::decay_t<Handler>>
{
    return {std::forward<Handler>(handler)};
}

template <typename Handler>
auto post(Handler &&handler)
    -> detail::work_poster<std::decay_t<Handler>>
{
    return {std::forward<Handler>(handler)};
}

} // namespace network
} // namespace kr