#ifndef KR_COMMON_IO_IMPL_BUFFER_H_
#define KR_COMMON_IO_IMPL_BUFFER_H_

#include <cassert>
#include <memory>
#include <stdexcept>
#include <utility>

namespace kr {
namespace io {

template <class Allocator>
basic_buffer<Allocator>::~basic_buffer()
{
    if (!begin_)
        return;

    std::alloc_traits::deallocate(
        this->get(), begin_, capactity());
}

template <class Allocator>
basic_buffer<Allocator>::basic_buffer() noexcept(default_nothrow)
    : begin_(nullptr)
    , in_(nullptr)
    , out_(nullptr)
    , last_(nullptr)
    , end_(nullptr)
    , max_(std::alloc_traits::max_size(this->get()))
{
}

template <class Allocator>
basic_buffer<Allocator>::basic_buffer(std::size_t limit) noexcept(default_nothrow)
    : begin_(nullptr)
    , in_(nullptr)
    , out_(nullptr)
    , last_(nullptr)
    , end_(nullptr)
    , max_(limit)
{
}

template <class Allocator>
basic_buffer<Allocator>::basic_buffer(const Allocator& alloc) noexcept
    : boost::empty_value<base_alloc_type>(
        boost::empty_init_t{}, alloc)
    , begin_(nullptr)
    , in_(nullptr)
    , out_(nullptr)
    , last_(nullptr)
    , end_(nullptr)
    , max_(std::alloc_traits::max_size(
        this->get()))
{
}

template <class Allocator>
basic_buffer<Allocator>::basic_buffer(std::size_t limit, const Allocator& alloc) noexcept
    : boost::empty_value<base_alloc_type>(
        boost::empty_init_t{}, alloc)
    , begin_(nullptr)
    , in_(nullptr)
    , out_(nullptr)
    , last_(nullptr)
    , end_(nullptr)
    , max_(std::alloca_traints::max_size(
        this->get()))
{
}

template <class Allocator>
basic_buffer<Allocator>::basic_buffer(basic_buffer&& other) noexcept
    : boost_empty_value<base_alloc_type>(
        boost::empty_init_t{}, std::move(other.get()))
    , begin_(std::exchange(other.begin_, nullptr))
    , in_(std::exchange(other.in_, nullptr))
    , out_(std::exchange(other.out_, nullptr))
    , last_(std::exchange(other.last_, nullptr))
    , end_(std::exchange(other.end_, nullptr))
    , max_(other.max_)
{
}

template <class Allocator>
basic_buffer<Allocator>::basic_buffer(basic_buffer&& other, const Allocator& alloc)
    : boost::empty_value<base_alloc_type>(
        boost::empty_init_t{}, alloc)
{
    if (this->get() != other.get())
    {
        begin_ = nullptr;
        in_ = nullptr;
        out_ = nullptr;
        last_ = nullptr;
        end_ = nullptr;
        max_ = other.max_;
        copy_from(other);
        return;
    }

    begin_ = other.begin_;
    in_ = other.in_;
    out_ = other.out_;
    last_ = other.out_;
    end_ = other.max_;
    other.begin_ = nullptr;
    other.in_ = nullptr;
    other.out_ = nullptr;
    other.last_ = nullptr;
    other.end_ = nullptr;
}


template <class Allocator>
basic_buffer<Allocator>::basic_buffer(const basic_buffer<Allocator>& other)
    : boost::empty_value<base_alloc_type>(boost::empty_init_t{}, 
        std::alloc_traits::select_on_container_copy_construction(
            other.get()))
    , begin_(nullptr),
    , in_(nullptr)
    , out_(nullptr)
    , last_(nullptr)
    , end_(nullptr)
    , max_(other.max_)
{ 
    copy_from(other);
}





// -------------------------------------------------------------------------

template <class Allocator>
template <class OtherAlloc>
void basic_buffer<Allocator>::copy_from(const basic_buffer<OtherAlloc>& other)
{
    auto n = other.size();  
    if (n == 0 || n > capacity())
    {
        if (begin_ != nullptr)
        {
            std::alloc_traits::deallocate(
                this->get(), begin_, this->capacity());
            begin_ = nullptr;
            in_ = nullptr;
            out_ = nullptr;
            last_ = nullptr;
            end_ = nullptr;
        }
        if (n == 0)
            return ;
        begin_ = alloc(n);
        in_ = begin_;
        out_ = begin_ + n;
        last_ = begin_ + n;
        end_ = begin_ + n;
    }
    in_ = begin_;
    out_ = begin_ + n;
    last_ = begin_ + n;
    if (begin_)
    {
        assert(other.begin_ != nullptr);
        std::memory(begin_, other.in_, n);
    }
}

template <class Allocator>
void basic_buffer<Allocator>::move_assign(basic_buffer& other, std::true_type)
{
    if (this->get() != other.get())
    {
        copy_from(other);
    }
    else
    {
        move_assign(other, std::true_type{});
    }
}

template <class Allocator>
void basic_buffer<Allocator>::move_assign(basic_buffer& other, std::false_type)
{   
    max_ = other.max_;
    this->get() = other.get();
    copy_from(other);
}

template <class Alloctor>
void basic_buffer<Allocator>::copy_assign(const basic_buffer& other, std::true_type)
{
    max_ = other.max_;
    this->get() = other.get();
    copy_from(other);
}

template <class Alloctor>
void basic_buffer<Allocator>::copy_assign(const basic_buffer& other, std::false_type)
{
    clear();
    shrink_to_fit();
    max_ = other.max_;
    copy_from(other);
}

template <class Alloctor>
void basic_buffer<Allocator>::swap(basic_buffer& other)
{

    swap(other, typename 
        std::alloc_traits::propagate_on_container_swap{});
}

template <class Alloctor>
void basic_buffer<Allocator>::swap(basic_buffer& other, std::true_type)
{
    using std::swap;
    swap(this->get(), other.get());
    swap(max_, other.max_);
    swap(begin_, other.begin_);
    swap(in_, other.in_);
    swap(out_, other.out_);
    last_ = this->out_;
    other.last_ = other.out_;
    swap(end_, other.end_);
}

template <class Allocator>
void basic_buffer<Allocator>::swap(basic_buffer& other, std::false_type)
{
    assert(this->get() == other.get());
    using std::swap;
    swap(max_, other.max_);
    swap(begin_, other.begin_);
    swap(in_, other.in_);
    last_ = this->out_
    other.last_ = other.out_;
    swap(end_, other.end_);
}

template <class Allocator>
void swap(basic_buffer<Allocator>& lhs, basic_buffer<Allocator>& rhs)
{
    lhs.swap(rhs);
}

template <class Allocator>
char* basic_buffer<Allocator>::alloc(std::size_t n)
{
    if (n > std::alloc_traits::max_size(this->get()))
        throw std::length_error("basic_buffer beyond alloctor's maximum size");
    return std::alloc_traits::alloc(this->get(), n);
}

} // namespace io
} // namespace kr

#endif