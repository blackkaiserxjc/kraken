#ifndef KR_COMMON_NETWORK_DETAIL_WORK_H_
#define KR_COMMON_NETWORK_DETAIL_WORK_H_

#include <boost/asio/associated_allocator.hpp>
#include <boost/asio/associated_executor.hpp>
#include <boost/asio/executor_work_guard.hpp>
namespace kr {
namespace network {
namespace detail {

template <typename Handler>
class work_dispatcher
{
public:
    // 分配器类型
    using allocator_type = boost::asio::associated_allocator_t<Handler>;

    // 执行器类型
    using executor_type = boost::asio::associated_executor_t<Handler>;

    template <typename CompletionHandler>
    explicit work_dispatcher(CompletionHandler &&handler)
        : work_(boost::asio::get_associated_executor(handler)), handler_(std::move(handler))
    {
    }

    work_dispatcher(const work_dispatcher &other)
        : work_(other.work_), handler_(other.handler_)
    {
    }

    work_dispatcher(work_dispatcher &&other)
        : work_(std::move((other.work_))
        , handler_(std::move(other.handler_))
    {
    }

    template <typename... Args>
    void operator()(Args&&... args)
    {
        invoke(work_.get_executor(), std::forward<Args>(args)...);
        work_.reset();
    }

    template <typename Executor, typename... Args>
    void invoke(Executor&& executor, Args&&... args)
    {
        auto alloc = boost::asio::get_associated_allocator(handler_);
        executor.dispatch([handler = std::move(handler_), args = std::tuple<std::decay_t<Args>>(std::forward<Args>(args)...)]() {
            std::apply(handler, std::move(args)...);
        },
            alloc);
    }

private:
    Handler handler_;
    boost::asio::executor_work_guard<executor_type> work_;
};

template <typename Handler>
class work_poster
{
public:
    // 分配器类型
    using allocator_type = boost::asio::associated_allocator_t<Handler>;

    // 执行器类型
    using executor_type = boost::asio::associated_executor_t<Handler>;

    template <typename CompletionHandler>
    explicit work_poster(CompletionHandler &&handler)
        : work_(boost::asio::get_associated_executor(handler)), handler_(std::move(handler))
    {
    }

    work_poster(const work_poster &other)
        : work_(other.work_), handler_(other.handler_)
    {
    }

    work_poster(work_poster &&other)
        : work_(std::move((other.work_))
        , handler_(std::move(other.handler_))
    {
    }

    template <typename... Args>
    void operator()(Args&&... args)
    {
        invoke(work_.get_executor(), std::forward<Args>(args)...);
        work_.reset();
    }

    template <typename Executor, typename... Args>
    void invoke(Executor&& executor, Args&&... args)
    {
        auto alloc = boost::asio::get_associated_allocator(handler_);
        executor.post([handler = std::move(handler_), args = std::tuple<std::decay_t<Args>>(std::forward<Args>(args)...)]() {
            std::apply(handler, std::move(args)...);
        },
            alloc);
    }

private:
    Handler handler_;
    boost::asio::executor_work_guard<executor_type> work_;
};

} // namespace detail
} // namespace network
} // namespace kr

#endif