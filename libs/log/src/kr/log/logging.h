//
// Created by kaiser on 2021/1/2.
//

#ifndef KR_COMMON_LOG_LOGGING_H_
#define KR_COMMON_LOG_LOGGING_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <cstdint>
#include <iosfwd>
#include <ostream>
#include <string>
#include <type_traits>

#include <boost/log/common.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace kr {
namespace log {

// 日志级别
enum class severity_level : std::uint32_t
{
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

// 日志级别转换为流
template <typename CharT, typename TraitsT>
inline std::basic_ostream<CharT, TraitsT>& operator << (
        std::basic_ostream<CharT, TraitsT>& strm, severity_level level);

// 流转换为日志级别
template <typename CharT, typename TraitsT>
inline std::basic_istream<CharT, TraitsT>& operator >> (
        std::basic_istream<CharT, TraitsT>& strm, severity_level& level);


// 非线程安全日志器
class logger : public boost::log::sources::severity_channel_logger<severity_level, std::string>
{
public:
    /**
     * 构造函数
     * @param channel 通道
     */
    explicit logger(const std::string &channel);

    /**
     * 设置属性值
     * @param name  属性名
     * @param value 属性值
     */
    template <typename ValueType>
    ValueType set_attr_value(const char* name, ValueType value);

    /**
     *  获取全局日志对象
     * @return 全局日志对象
     */
    static logger &get();
};

// 线程安全日志器
class logger_mt : public boost::log::sources::severity_channel_logger_mt<severity_level, std::string>
{
public:
    /**
     * 构造函数
     * @param channel 通道
     */
    explicit logger_mt(const std::string &channel);

    /**
     * 设置属性值
     * @param name  属性名
     * @param value 属性值
     */
    template <typename ValueType>
    ValueType set_attr_value(const char* name, ValueType value);

    /**
     *  获取全局日志对象
     * @return 全局日志对象
     */
    static logger_mt &get();
};

/** 日志宏实现 */
#define KRLOG_SEV_IMPL(logger, level, file, line, tag) \
    BOOST_LOG_STREAM_WITH_PARAMS((logger), \
        ((logger).set_attr_value("File", static_cast<std::string>(file))) \
        ((logger).set_attr_value("Line", static_cast<int>(line))) \
        ((logger).set_attr_value("Tag", static_cast<std::string>(tag))) \
        (boost::log::keywords::severity = (level)) \
    )

/** 日志宏 **/
#define KRLOG_SEV(logger, level, tag) KRLOG_SEV_IMPL(logger, level, __FILE__, __LINE__, tag)

/** 跟踪日志宏 **/
#define KRLOG_TRACE(logger, tag) KRLOG_SEV(logger, kr::log::severity_level::TRACE, tag)

/** 调试日志宏 **/
#define KRLOG_DEBUG(logger, tag) KRLOG_SEV(logger, kr::log::severity_level::DEBUG, tag)

/** 信息日志宏 **/
#define KRLOG_INFO(logger, tag) KRLOG_SEV(logger, kr::log::severity_level::INFO, tag)

/** 警告日志宏 **/
#define KRLOG_WARN(logger, tag) KRLOG_SEV(logger, kr::log::severity_level::WARN, tag)

/** 错误日志宏 **/
#define KRLOG_ERROR(logger, tag) KRLOG_SEV(logger, kr::log::severity_level::ERROR, tag)

/** 严重错误日志宏 **/
#define KRLOG_FATAL(logger, tag) KRLOG_SEV(logger, kr::log::severity_level::FATAL, tag)

} // logging
} // kr

#include <kr/log/detail/logging.h>
#endif //KR_COMMON_LOG_LOGGING_H_
