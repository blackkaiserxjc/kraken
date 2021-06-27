//
// Created by kaiser on 2021/1/1.
//

#ifndef KR_COMMON_UITILITY_DETAIL_H_
#define KR_COMMON_UITILITY_DETAIL_H_

#include <fmt/format.h>

namespace kr {
namespace utility {

template <class Logger, class Clock>
basic_auto_timer<Logger, Clock>::basic_auto_timer(
    std::string &&msg,
    const value_type &min_time_to_log,
    Logger &&logger)
    : message_(std::move(msg)),
      min_time_to_log_(min_time_to_log),
      logger_(std::move(logger))
{
}

template <class Logger, class Clock>
basic_auto_timer<Logger, Clock>::~basic_auto_timer()
{
    if (message_)
    {
        log(message_.value());
    }
}

template <class Logger, class Clock>
auto basic_auto_timer<Logger, Clock>::log(std::string_view msg)
{
    return log_impl(Clock::now(), msg);
}

template <class Logger, class Clock>
template <class Format, class... Args>
auto basic_auto_timer<Logger, Clock>::log_format(Format &&format, Args &&...args)
{
    auto now = Clock::now();
    return log_impl(now, fmt::format(std::forward<Format>(format), std::forward<Args>(args)...));
}

template <class Logger, class Clock>
auto basic_auto_timer<Logger, Clock>::log_impl(std::chrono::time_point<Clock> now, std::string_view msg)
{
    auto duration = now - start_;
    if (duration >= min_time_to_log_)
    {
        logger_(msg, duration);
    }
    start_ = Clock::now();
    return duration;
}

} // namespace utility
} // namespace kr

#endif //KR_COMMON_UITILITY_DETAIL_H_
