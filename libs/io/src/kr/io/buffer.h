//
// Created by kaiser on 2021/5/4.
//

#ifndef KR_COMMON_IO_BUFFER_H_
#define KR_COMMON_IO_BUFFER_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <boost/core/empty_value.hpp>
#include <limits>
#include <memory>
#include <type_traits>

namespace kr {
namespace io {

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
    using const_buffers_type = boost::asio::const_buffer;
    using mutable_buffers_type = boost::asio::mutable_buffer;

    /**
     *  无参构造函数
     */
    basic_buffer() noexcept(default_nothrow);

    /**
     * 构造函数
     * @param limit 大小限制
     */
    explicit basic_buffer(std::size_t limit) noexcept(default_nothrow);

    /**
     * 构造函数
     * @param alloc 分配器
     */
    explicit basic_buffer(const Allocator &alloc) noexcept;

    /**
     * 构造函数
     * @param limit 限制
     * @param alloc 分配器
     */
    basic_buffer(std::size_t limit, const Allocator &alloc) noexcept;

    /**
     * 移动构造
     * @param other
     */
    basic_buffer(basic_buffer &&other) noexcept;
    basic_buffer(basic_buffer &&other, const Allocator &alloc);

    /**
     * 复制构造
     * @param other
     */
    basic_buffer(const basic_buffer &other);
    basic_buffer(const basic_buffer &other, const Allocator &alloc);
    template <class OtherAlloc>
    basic_buffer(const basic_buffer<OtherAlloc> &other) noexcept(default_nothrow);
    template <class OtherAlloc>
    basic_buffer(const basic_buffer<OtherAlloc> &other, const Allocator &alloc);

    /**
     * 移动赋值
     * @param other
     * @return
     */
    basic_buffer &operator=(basic_buffer &&other) noexcept;

    /**
     * 赋值
     * @param other
     * @return
     */
    basic_buffer &operator=(const basic_buffer &other);
    template <class OtherAlloc>
    basic_buffer &operator=(const basic_buffer<OtherAlloc> &other);

    /**
     * 获取分配器
     * @return 分配器
     */
    allocator_type get_allocator() const
    {
        return this->get();
    }

    /**
     * 设置max_size
     * @param n
     */
    void max_size(std::size_t n) noexcept
    {
        max_ = n;
    }

    /**
     * 预分配
     * @param n
     */
    void reserve(std::size_t n);

    /**
     * 移除未使用的容量
     */
    void shrink_to_fit() noexcept;

    /**
     * 清空
     */
    void clear() noexcept;

    template <class Alloc>
    friend void swap(basic_buffer<Alloc> &, basic_buffer<Alloc> &);

    /**
     * 获取可写大小
     * @return 可写大小
     */
    std::size_t size() const noexcept
    {
        return dist(in_, out_);
    }

    /**
     * 获取最大size
     * @return size
     */
    std::size_t max_size() const noexcept
    {
        return max_;
    }

    /**
     * 获取容量
     * @return 容量
     */
    std::size_t capacity() const noexcept
    {
        return dist(begin_, end_);
    }

    /**
     * 获取数据
     * @return
     */
    const_buffers_type data() const noexcept
    {
        return {in_, dist(in_, out_)};
    }

    /**
     * 获取数据
     * @return
     */
    mutable_buffers_type data() noexcept
    {
        return {in_, dist(in_, out_)};
    }

    /**
     *  分配内存
     * @param n
     * @return 数据
     */
    mutable_buffers_type prepare(std::size_t n);

    /**
     * 数据提交
     * @param n
     */
    void commit(std::size_t n) noexcept
    {
        out_ += std::min(n, dist(out_, last_));
    }

    /**
     * 数据消费
     * @param n
     */
    void consume(std::size_t n) noexcept;

private:
    static std::size_t dist(const char *first, const char *last)
    {
        return static_cast<std::size_t>(last - first);
    }

    template <class OtherAlloc>
    void copy_from(const basic_buffer<OtherAlloc> &other);
    void move_assign(basic_buffer &, std::true_type);
    void move_assign(basic_buffer &, std::false_type);
    void copy_assign(const basic_buffer &, std::true_type);
    void copy_assign(const basic_buffer &, std::false_type);
    void swap(basic_buffer &);
    void swap(basic_buffer &, std::true_type);
    void swap(basic_buffer &, std::false_type);
    char *alloc(std::size_t n);

    char *begin_;
    char *in_;
    char *out_;
    char *last_;
    char *end_;
    std::size_t max_;
};

using buffer = basic_buffer<std::allocator<char>>;

} // namespace io
} // namespace kr

#include <kr/io/impl/buffer.h>

#endif
