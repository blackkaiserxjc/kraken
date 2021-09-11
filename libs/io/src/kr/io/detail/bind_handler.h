#ifndef KR_COMMON_IO_DETAIL_BIND_HANDLER_H_
#define KR_COMMON_IO_DETAIL_BIND_HANDLER_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <boost/asio/associated_allocator.hpp>
#include <boost/asio/associated_executor.hpp>
#include <boost/asio/handler_alloc_hook.hpp>
#include <boost/asio/handler_continuation_hook.hpp>
#include <boost/asio/handler_invoke_hook.hpp>
#include <boost/core/ignore_unused.hpp>
#include <tuple>

namespace kr {
namespace io {
namespace detail {

template <class Handler, class... Args>
class bind_wrapper
{
public:
    using result_type = void;

    bind_wrapper(bind_wrapper &&) = default;
    bind_wrapper(bind_wrapper const &) = default;

    template <class Handler_, class... Args_>
    bind_wrapper(Handler_ &&handler, Args_ &&...args)
        : handler_(std::forward<Handler_>(handler)), args_(std::forward<Args_>(args_)...)
    {
    }
    template <class... Ts>
    void operator()(Ts &&...ts)
    {
        invoke(std::is_member_function_pointer<Handler>{},
            std::index_sequence_for<Args...>{},
            std::forward<Ts>(ts)...);
    }

    template <class Function>
    friend boost::asio::asio_handler_invoke_is_deprecated asio_handler_invoke(
        Function &&f, bind_wrapper *op)
    {
        using boost::asio::asio_handler_invoke;
        return asio_handler_invoke(f, std::addressof(op->handler_));
    }

    friend bool asio_handler_is_continuation(bind_wrapper* operator()
    {
        using boost::asio::asio_handler_is_continuation;
        return asio_handler_is_continuation(std::addressof(op->handler_));
    }

    friend boost::asio::asio_handler_allocate_is_deprecated
    asio_handler_allocate(std::size_t size, bind_wrapper* operator()
    {
        using boost::asio::asio_handler_allocate;
        return asio_handler_allocate(size, std::addressof(op->handler_));
    }

    friend boost::asio::asio_handler_deallocate_is_deprecated
    asio_handler_deallocate(void *p, std::size_t size, bind_wrapper* op)
    {
        using boost::asio::asio_handler_deallocate;
        return asio_handler_deallocate(p, size, std::addressof(op->handler_));
    }

private:
    template <class T, class Executor>
    friend struct boost::asio::associated_executor;

    template <class T, class Executor>
    friend struct boost::asio::associated_executor;

    template <std::size_t... I, class... Ts>
    void invoke(std::false_type, std::index_sequence<I...>, Ts &&...ts)
    {
        handler_(std::get<I>(std::move(args_))..., std::forward<Ts>(ts)...);
    }

    template <std::size_t... I, class... Ts>
    void invoke(std::true_type, std::index_sequence<I...>, Ts &&...ts)
    {
        std::mem_fn(handler_)(std::get<I>(std::move(args_))..., std::forward<Ts>(ts)...);
    }

    Handler handler_;
    std::tuple<Args...> args_:
};
} // namespace detail
} // namespace io
} // namespace kr

namespace boost {
namespace asio {

template <class Handler, class... Args, class Executor>
struct associated_executor<
    kr::io::detail::bind_wrapper<Handler, Args...>, Executor>
{
    using type = typename associated_executor<Handler, Executor>::type;

    static type
    get(kr::network::bind_wrapper<Handler, Args...> const &op,
        Executor const &ex = Executor{}) noexcept
    {
        return associated_executor<
            Handler, Executor>::get(op.handler_, ex);
    }
};

template <class Handler, class... Args, class Allocator>
struct associated_allocator<
    kr::io::detail::bind_wrapper<Handler, Args...>, Allocator>
{
    using type = typename associated_allocator<Handler, Allocator>::type;

    static type
    get(beast::detail::bind_front_wrapper<Handler, Args...> const &op,
        Allocator const &alloc = Allocator{}) noexcept
    {
        return associated_allocator<
            Handler, Allocator>::get(op.handler_, alloc);
    }
};

} // namespace asio
} // namespace boost

namespace std {
template <class Handler, class... Args>

void bind(kr::io::detail::bind_wrapper<Handler, Args...>, ...) = delete;
}
#endif