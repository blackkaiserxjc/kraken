#ifndef KR_COMMON_PROTOBUF_H_
#define KR_COMMON_PROTOBUF_H_

#include <functional>

#include <boost/asio/buffer.hpp>
#include <google/protobuf/message.h>

namespace kr {
namespace protobuf {

enum class Errors
{
    OK = 0,
    TimeOut = 1,
};

template <typename... Args>
class service;

template <typename... Args>
class channel
{
public:
    /**
     * 构造函数 
     */
    inline channel() = default;

    /**
     * 析构函数
     */
    virtual ~channel() = default;

    /**
     * 服务注册 
     * @param service 服务
     */
    virtual void register_service(service<Args...> &service) = 0;

    /**
     * 取消服务
     * @param service 服务 
     */
    virtual void unregister_service(service<Args...> &service) = 0;

    /**
     * 
     * 
     */
    virtual void call_method(
        const Args &args,
        int method,
        const google::protobuf::Message &input,
        std::function<void(Errors, boost::asio::const_buffer)> done,
        std::chrono::milliseconds timeout) = 0;
};

template <typename... Args>
class service
{
public:
    /**
     * 默认构造 
     */
    inline service() = default;

    /**
     * 析构函数 
     */
    virtual ~service() = default;

    /**
     * 禁用复制构造和赋值 
     */
    service(const service &) = delete;
    service &operator=(const service &) = delete;

    /**
     *  调用服务方法
     * @param args 参数
     * @param method 方法id
     * @param input 输入
     * @param done 回调 
     */
    virtual void call_method(
        const Args &...args,
        int method,
        boost::asio::const_buffer input,
        std::function<void(const google::protobuf::Message &)> done) = 0;
};

} // namespace protobuf
} // namespace kr
#endif