//
// Created by kaiser on 2021/5/4.
//

#ifndef KR_COMMON_NET_BUFFER_H_
#define KR_COMMON_NET_BUFFER_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <boost/core/empty_value.hpp>
#include <limits>
#include <memory>
#include <type_traits>

namespace kr {
namespace net {

template <class Allocator>
class basic_buffer : private boost::empty_value<
                         typename std::allocator_traits<Allocator>::
                             template rebind_alloc<char>>
{
    template <class OtherAlloc>
    friend class buffer;

    using base_alloc_type = typename std::allocator_traits<Allocator>::
        template rebind_alloc<char>;

    static bool constexpr default_nothrow =
        std::is_nothrow_default_constructible<Allocator>::value;

    using alloc_traits = std::allocator_traits<base_alloc_type>;

    using pocma = typename alloc_traits::propagate_on_container_move_assignment;

    using pocca = typename alloc_traits::propagate_on_container_copy_assignment;

public:
    using allocator_type = Allocator;

    basic_buffer() noexcept(default_nothrow);

    explicit basic_buffer(std::size_t limit) noexcept(default_nothrow);

    explicit basic_buffer(const Allocator &alloc) noexcept;

private:
    static std::size_t dist(const char *first, const char *last)
    {
        return static_cast<std::size_t>(last - first);
    }

    char *begin_;
    char *in_;
    char *out_;
    char *last_;
    char *end_;
    std::size_t max_;
};

} // namespace net
} // namespace kr
#endif
