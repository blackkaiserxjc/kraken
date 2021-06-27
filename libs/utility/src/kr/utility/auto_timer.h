#ifndef KR_COMMON_UTILITY_AUTO_TIMER_H_
#define KR_COMMON_UTILITY_AUTO_TIMER_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <chrono>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#include <kr/log/logging.h>

namespace kr {
namespace utility {

// 执行时间分析工具类
template <class Logger, class Clock>
class basic_auto_timer final
{
public:
    using value_type = std::chrono::duration<double>;

    /**
     * 构造函数
     * @param msg
     * @param min_time_to_log
     * @param logger
     */
    explicit basic_auto_timer(std::string &&msg = "",
        const value_type &min_time_to_log = value_type::zero(),
        Logger &&logger = Logger());

    /** 析构函数 **/
    ~basic_auto_timer();

    /** 禁用复制和赋值 **/
    basic_auto_timer(const basic_auto_timer &) = delete;
    basic_auto_timer &operator=(const basic_auto_timer &) = delete;

    /**
     * 日志
     * @param msg   信息
     * @return      间隔时间
     */
    auto log(std::string_view msg = "");

    /**
     * 格式化日志
     * @param format 格式串
     * @param args   变参数集
     * @return       间隔时间
     */
    template <class Format, class... Args>
    auto log_format(Format &&format, Args &&...args);

private:
    // 日志实现
    auto log_impl(std::chrono::time_point<Clock> now, std::string_view msg);

    // 日志信息
    std::optional<std::string> message_;

    // 开始时间点
    std::chrono::time_point<Clock> start_;

    // 最小间隔时间
    value_type min_time_to_log_;

    // 日志器
    Logger logger_;
};

// 默认日志器
struct logger final
{
    /**
     * 构造函数
     */
    logger();
    /**
     * 重载括号
     * @param msg  日志信息
     * @param sec  时间间隔
     */
    void operator()(std::string_view msg, const std::chrono::duration<double> &sec) const;

    // 日志器
    mutable kr::log::logger_mt logger_;
};

// 外部使用类型
template <class Logger, class Clock>
using auto_timer = basic_auto_timer<logger, std::chrono::high_resolution_clock>;

// 生产器
template <
    class Logger = logger,
    class Clock = std::chrono::high_resolution_clock>
auto make_auto_timer(std::string &&msg = "",
    const std::chrono::duration<double> &min_time_to_log =
        std::chrono::duration<double>::zero(),
    Logger &&logger = Logger())
{
    return basic_auto_timer<Logger, Clock>(std::move(msg), min_time_to_log, std::move(logger));
}

} // namespace utility
} // namespace kr

#include <kr/utility/detail/auto_timer.h>

#endif
