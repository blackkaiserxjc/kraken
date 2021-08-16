#pragma once

#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>

#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index_container.hpp>

namespace kr {
namespace utility {

/**
 * ioc接口
 */
class ioc
{
public:
    /**
     * 构造函数
     */
    ioc() = default;

    /**
     * 析构函数
     */
    ~ioc() = default;

    /**
     * 创建回调
     */
    virtual void on_initialize() = 0;

    /**
     * 析构回调
     */
    virtual void on_destroy() = 0;
};

class ioc_container
{
public:
    /**
     * 构造函数
     */
    ioc_container() = default;
    /**
     * 析构函数
     */
    ~ioc_container();

    ioc_container(const ioc_container &) = delete;
    ioc_container &operator=(const ioc_container &) = delete;

    /**
     * 注册类
     * @param args 构造参数列表
     */
    template <typename T, typename... Args>
    void bind(Args &&...args);

    /**
     * 注册一个类
     * @param args 构造参数列表
     */
    template <typename T, typename D, typename... Args>
    void bind_as(Args &&...args);

    /**
     * 加载接口
     * @return 接口指针
     */
    template <typename T>
    std::shared_ptr<T> load();

    /**
     * 销毁对象
     */
    void destroy();

private:
    // Id
    struct Id;

    // 类型id
    template <typename T>
    struct TypeId;

    // 对象保存接口
    struct IHolder
    {
        const Id *id{};
        virtual void initialize(const Id &) = 0;
        virtual void destroy(const Id &) = 0;
    };

    // 普通对象
    template <typename T>
    struct Holder;

    // Ioc侵入对象接口
    template <typename T>
    struct IocHolder;

    // 对象创建器
    std::unordered_map<const Id *, std::function<std::shared_ptr<IHolder>()>> creators_;

    // 对象列表
    boost::multi_index_container<std::shared_ptr<IHolder>,
        boost::multi_index::indexed_by<
            boost::multi_index::sequenced<>,
            boost::multi_index::hashed_unique<boost::multi_index::member<IHolder, const Id *, &IHolder::id>>>>
        objects_;
};

} // namespace utility
} // namespace kr

#include <kr/utility/detail/ioc_container.h>
