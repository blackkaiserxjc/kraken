#pragma once

#include <algorithm>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace kr {

namespace core {

template <class T, class E>
class expected;

template <class E>
class unexpected
{
public:
    static_assert(!std::is_same_v<E, void>, "E must not be void");

    unexpected() = delete;
    constexpr explicit unexpected(const E &e)
        : val_(e)
    {
    }
    constexpr explicit unexpected(E &&e)
        : val_(std::move(e))
    {
    }

    constexpr const E &value() const &
    {
        return val_;
    }
    constexpr E &value() &
    {
        return val_;
    }
    constexpr E &&value() &&
    {
        return std::move(val_);
    }
    constexpr const E &&value() const &&
    {
        return std::move(val_);
    }

private:
    E val_;
};

template <class E>
constexpr bool operator==(const unexpected<E> &lhs, const unexpected<E> &rhs)
{
    return lhs.value() == rhs.value();
}

template <class E>
constexpr bool operator!=(const unexpected<E> &lhs, const unexpected<E> &rhs)
{
    return lhs.value() != rhs.value();
}

template <class E>
constexpr bool operator<(const unexpected<E> &lhs, const unexpected<E> &rhs)
{
    return lhs.value() < rhs.value();
}

template <class E>
constexpr bool operator<=(const unexpected<E> &lhs, const unexpected<E> &rhs)
{
    return lhs.value() <= rhs.value();
}

template <class E>
constexpr bool operator>(const unexpected<E> &lhs, const unexpected<E> &rhs)
{
    return lhs.value() > rhs.value();
}

template <class E>
constexpr bool operator>=(const unexpected<E> &lhs, const unexpected<E> &rhs)
{
    return lhs.value() >= rhs.value();
}

template <class E>
auto make_unexpected(E &&e)
{
    return unexpected<typename std::decay_t<E>>(std::forward<E>(e));
}

struct unexpected_t
{
    unexpected_t() = default;
};

inline constexpr unexpected_t unexpect{};

template <class E>
class bad_expected_access;

template <>
class bad_expected_access<void> : public std::exception
{
public:
    explicit bad_expected_access() = default;
};

template <class E>
class bad_expected_access : public bad_expected_access<void>
{
public:
    explicit bad_expected_access(E e)
        : val_(std::move(e))
    {
    }

    virtual const char *what() const noexcept override
    {
        return "bad expeted access";
    }

    E &error() &
    {
        return val_;
    }
    const E &error() const &
    {
        return val_;
    }
    E &&error() &&
    {
        return std::move(val_);
    }
    const E &&error() const &&
    {
        return std::move(val_);
    }

private:
    E val_;
};

namespace detail {
inline constexpr enum class value_tag_t {
} value_tag{};
inline constexpr enum class error_tag_t {
} error_tag{};

template <class T, std::enable_if_t<std::is_trivially_destructible<T>::value> * = nullptr>
void destroy(T &)
{
}

template <
    class T,
    std::enable_if_t<
        !std::is_trivially_destructible<T>::value && (std::is_class<T>::value || std::is_union<T>::value)> * = nullptr>
void destroy(T &t)
{
    t.~T();
}

template <class T, class E>
union constexpr_storage
{
    using value_type = T;
    using error_type = E;
    using unexpected_type = unexpected<E>;
    char dummy;
    value_type val;
    error_type err;

    constexpr constexpr_storage()
        : dummy()
    {
    }

    constexpr constexpr_storage(value_tag_t)
        : val()
    {
    }

    constexpr constexpr_storage(error_tag_t)
        : err()
    {
    }

    template <typename U = T>
    constexpr constexpr_storage(value_tag_t, U &&v)
        : val(std::forward<U>(v))
    {
    }

    template <typename U = E>
    constexpr constexpr_storage(error_tag_t, U &&e)
        : err(std::forward<U>(e))
    {
    }

    ~constexpr_storage() = default;
};

template <class T, class E>
union storage
{
    using value_type = T;
    using error_type = E;
    using unexpected_type = unexpected<E>;
    char dummy;
    value_type val;
    error_type err;

    constexpr storage()
        : dummy()
    {
    }

    constexpr storage(value_tag_t)
        : val()
    {
    }

    constexpr storage(error_tag_t)
        : err()
    {
    }

    constexpr storage(value_tag_t, const value_type &val)
        : val(val)
    {
    }

    constexpr storage(value_tag_t, value_type &&val)
        : val(std::forward<value_type>(val))
    {
    }

    constexpr storage(error_tag_t, const error_type &err)
        : err(err)
    {
    }

    constexpr storage(error_tag_t, error_type &&err)
        : err(std::forward<error_type>(err))
    {
    }

    ~storage(){};
};

template <class E>
union constexpr_storage<void, E>
{
    using value_type = void;
    using error_type = E;
    using unexpected_type = unexpected<E>;
    char dummy;
    error_type err;

    constexpr constexpr_storage()
        : dummy()
    {
    }

    constexpr constexpr_storage(value_tag_t)
        : dummy()
    {
    }

    constexpr constexpr_storage(error_tag_t)
        : err()
    {
    }

    constexpr constexpr_storage(error_tag_t, const error_type &e)
        : err(e)
    {
    }

    ~constexpr_storage() = default;
};

template <class E>
union storage<void, E>
{
    using value_type = void;
    using error_type = E;
    using unexpected_type = unexpected<E>;
    char dummy;
    error_type err;

    constexpr storage()
        : dummy()
    {
    }

    constexpr storage(value_tag_t)
        : dummy()
    {
    }

    constexpr storage(error_tag_t)
        : err()
    {
    }

    constexpr storage(error_tag_t, const error_type &err)
        : err(err)
    {
    }

    constexpr storage(error_tag_t, error_type &&err)
        : err(std::forward<error_type>(err))
    {
    }

    ~storage(){};
};

template <class T, class E>
struct constexpr_base
{
    using value_type = T;
    using error_type = E;
    using unexpected_type = unexpected<E>;
    constexpr_storage<value_type, error_type> s;
    bool has;

    constexpr constexpr_base()
        : s(), has(true)
    {
    }

    constexpr constexpr_base(value_tag_t tag)
        : s(tag), has(true)
    {
    }
    constexpr constexpr_base(error_tag_t tag)
        : s(tag), has(false)
    {
    }
    template <typename U = T>
    constexpr constexpr_base(value_tag_t tag, U &&val)
        : s(tag, std::forward<U>(val)), has(true)
    {
    }

    template <typename U = E>
    constexpr constexpr_base(error_tag_t tag, U &&val)
        : s(tag, std::forward<U>(val)), has(false)
    {
    }

    ~constexpr_base() = default;
};

template <class T, class E>
struct base
{
    using value_type = T;
    using error_type = E;
    using unexpected_type = unexpected<E>;
    storage<value_type, error_type> s;
    bool has;

    constexpr base()
        : s(), has(true)
    {
    }

    constexpr base(value_tag_t tag)
        : s(tag), has(true)
    {
    }

    constexpr base(error_tag_t tag)
        : s(tag), has(false)
    {
    }

    constexpr base(value_tag_t tag, const value_type &val)
        : s(tag, val), has(true)
    {
    }

    constexpr base(value_tag_t tag, value_type &&val)
        : s(tag, std::forward<value_type>(val)), has(true)
    {
    }

    constexpr base(error_tag_t tag, const error_type &err)
        : s(tag, err), has(false)
    {
    }

    constexpr base(error_tag_t tag, error_type &&err)
        : s(tag, std::forward<error_type>(err)), has(false)
    {
    }

    base(const base &o)
        : has(o.has)
    {
        if (has)
            ::new (std::addressof(s.val)) value_type(o.s.val);
        else
            ::new (std::addressof(s.err)) error_type(o.s.err);
    }

    base(base &&o) noexcept
        : has(o.has)
    {
        if (has)
            ::new (std::addressof(s.val)) value_type(std::move(o.s.val));
        else
            ::new (std::addressof(s.err)) error_type(std::move(o.s.err));
    }

    ~base()
    {
        if (has)
            destroy(s.val);
        else
            destroy(s.err);
    }
};

template <class E>
struct constexpr_base<void, E>
{
    using value_type = void;
    using error_type = E;
    using unexpected_type = unexpected<E>;
    constexpr_storage<value_type, error_type> s;
    bool has;

    constexpr constexpr_base()
        : s(), has(true)
    {
    }

    constexpr constexpr_base(value_tag_t tag)
        : s(tag), has(true)
    {
    }

    constexpr constexpr_base(error_tag_t tag)
        : s(tag), has(false)
    {
    }

    constexpr constexpr_base(error_tag_t tag, const error_type &err)
        : s(tag, err), has(false)
    {
    }

    constexpr constexpr_base(error_tag_t tag, error_type &&err)
        : s(tag, std::forward<error_type>(err)),
          has(false)
    {
    }

    ~constexpr_base() = default;
};

template <class E>
struct base<void, E>
{
    using value_type = void;
    using error_type = E;
    using unexpected_type = unexpected<E>;
    storage<value_type, error_type> s;
    bool has;

    constexpr base()
        : s(), has(true)
    {
    }

    constexpr base(value_tag_t tag)
        : s(tag), has(true)
    {
    }

    constexpr base(error_tag_t tag)
        : s(tag), has(false)
    {
    }

    constexpr base(error_tag_t tag, const error_type &err)
        : s(tag, err), has(false)
    {
    }

    constexpr base(error_tag_t tag, error_type &&err)
        : s(tag, std::forward<error_type>(err)), has(false)
    {
    }

    base(const base &o)
        : has(o.has)
    {
        if (!has)
            ::new (std::addressof(s.err)) error_type(o.err);
    }

    base(base &&o) noexcept
        : has(o.has)
    {
        if (!has)
            ::new (std::addressof(s.err)) error_type(std::move(o.err));
    }

    ~base()
    {
        if (!has)
            destroy(s.err);
    }
};

template <class T, class E>
using base_select = typename std::conditional_t<
    ((std::is_void<T>::value || std::is_trivially_destructible<T>::value) && std::is_trivially_destructible<E>::value),
    constexpr_base<typename std::remove_const_t<T>, typename std::remove_const_t<E>>,
    base<typename std::remove_const_t<T>, typename std::remove_const_t<E>>>;
} // namespace detail

template <class T, class E>
class expected : private detail::base_select<T, E>
{
private:
    using base = detail::base_select<T, E>;
    using type = expected<typename base::value_type, typename base::error_type>;

public:
    using value_type = typename base::value_type;
    using error_type = typename base::error_type;
    using unexpected_type = typename base::unexpected_type;

    template <class U>
    struct rebind
    {
        using type = expected<U, error_type>;
    };
    constexpr expected()
        : base(detail::value_tag)
    {
    }
    expected(const expected &) = default;
    expected(expected &&) = default;

    constexpr expected(const value_type &e)
        : base(detail::value_tag, e)
    {
    }

    constexpr expected(value_type &&e)
        : base(detail::value_tag, std::forward<value_type>(e))
    {
    }

    template <class... Args>
    constexpr explicit expected(std::in_place_t, Args &&...args)
        : base(detail::value_tag, value_type(std::forward<Args>(args)...))
    {
    }

    constexpr expected(const unexpected_type &u)
        : base(detail::error_tag, u.value())
    {
    }

    constexpr expected(unexpected_type &&u)
        : base(detail::error_tag, std::forward<unexpected_type>(u).value())
    {
    }

    template <class Error>
    constexpr expected(const unexpected<Error> &u)
        : base(detail::error_tag, u.value)
    {
    }

    template <class Error>
    constexpr expected(unexpected<Error> &&u)
        : base(detail::error_tag, std::forward<Error>(u.value))
    {
    }

    template <class... Args>
    constexpr explicit expected(unexpected_t, Args &&...args)
        : base(detail::value_tag, unexpected_type(std::forward<Args>(args)...))
    {
    }

    ~expected() = default;

    expected &operator=(const expected &e)
    {
        type(e).swap(*this);
        return *this;
    }

    expected &operator=(expected &&e)
    {
        type(std::move(e)).swap(*this);
        return *this;
    }

    template <class U>
    expected &operator=(U &&u)
    {
        type(std::forward<U>(u)).swap(*this);
        return *this;
    }

    expected &operator=(const unexpected_type &u)
    {
        type(u).swap(*this);
        return *this;
    }

    expected &operator=(unexpected_type &&u)
    {
        type(std::move(u)).swap(*this);
        return *this;
    }

    void swap(expected &o)
    {
        using std::swap;
        if (base::has && o.has)
        {
            swap(base::s.val, o.s.val);
        }
        else if (base::has && !o.has)
        {
            error_type e(std::move(o.s.err));
            detail::destroy(o.s.err);
            ::new (std::addressof(o.s.val)) value_type(std::move(base::s.val));
            detail::destroy(base::s.val);
            ::new (std::addressof(base::s.err)) error_type(std::move(e));
            swap(base::has, o.has);
        }
        else if (!base::has & o.has)
        {
            value_type v(std::move(o.s.val));
            detail::destroy(o.s.val);
            ::new (std::addressof(o.s.err)) error_type(std::move(base::s.err));
            detail::destroy(base::s.err);
            ::new (std::addressof(base::s.val)) value_type(std::move(v));
            swap(base::has, o.has);
        }
        else
        {
            swap(base::s.err, o.s.err);
        }
    }

    constexpr const value_type *operator->() const
    {
        return &base::s.val;
    }

    value_type *operator->()
    {
        return &base::s.val;
    }

    constexpr const value_type &operator*() const &
    {
        return base::s.val;
    }

    value_type &operator*() &
    {
        return base::s.val;
    }

    constexpr const value_type &&operator*() const &&
    {
        return std::move(base::s.val);
    }

    constexpr value_type &&operator*() &&
    {
        return std::move(base::s.val);
    }

    constexpr explicit operator bool() const
    {
        return base::has;
    }

    constexpr bool has_value() const
    {
        return base::has;
    }

    constexpr const value_type &value() const &
    {
        if (!base::has)
            throw bad_expected_access<error_type>(base::s.err);
        return base::s.val;
    }

    constexpr value_type &value() &
    {
        if (!base::has)
            throw bad_expected_access<error_type>(base::s.err);
        return base::s.val;
    }

    constexpr const value_type &&value() const &&
    {
        if (!base::has)
            throw bad_expected_access<error_type>(base::s.err);
        return std::move(base::s.val);
    }

    constexpr value_type &&value() &&
    {
        if (!base::has)
            throw bad_expected_access<error_type>(base::s.err);
        return std::move(base::s.val);
    }

    constexpr const error_type &error() const &
    {
        if (base::has)
            throw bad_expected_access<void>();
        return base::s.err;
    }

    constexpr error_type &error() &
    {
        if (base::has)
            throw bad_expected_access<void>();
        return base::s.err;
    }

    constexpr const error_type &&error() const &&
    {
        if (base::has)
            throw bad_expected_access<void>();
        return std::move(base::s.err);
    }

    constexpr error_type &&error() &&
    {
        if (base::has)
            throw bad_expected_access<void>();
        return std::move(base::s.err);
    }

    template <class U>
    constexpr value_type value_or(U &&u) const &
    {
        return base::has ? **this : static_cast<value_type>(std::forward<U>(u));
    }

    template <class U>
    value_type value_or(U &&u) &&
    {
        return base::has ? std::move(**this) : static_cast<value_type>(std::forward<U>(u));
    }
};

template <class E>
class expected<void, E> : private detail::base_select<void, E>
{
private:
    using base = detail::base_select<void, E>;
    using type = expected<typename base::value_type, typename base::error_type>;

public:
    using value_type = typename base::value_type;
    using error_type = typename base::error_type;
    using unexpected_type = typename base::unexpected_type;

    template <class U>
    struct rebind
    {
        using type = expected<U, error_type>;
    };

    constexpr expected()
        : base(detail::value_tag)
    {
    }
    expected(const expected &) = default;
    expected(expected &&) = default;

    constexpr expected(unexpected_type const &u)
        : base(detail::error_tag, u.value())
    {
    }
    constexpr expected(unexpected_type &&u)
        : base(detail::error_tag, std::forward<unexpected_type>(u).value())
    {
    }
    template <class Error>
    constexpr expected(unexpected<Error> const &u)
        : base(detail::error_tag, u.value())
    {
    }

    ~expected() = default;

    expected &operator=(const expected &e)
    {
        type(e).swap(*this);
        return *this;
    }
    expected &operator=(expected &&e)
    {
        type(std::move(e)).swap(*this);
        return *this;
    }
    expected &operator=(const unexpected_type &u)
    {
        type(u).swap(*this);
        return *this;
    }

    expected &operator=(unexpected_type &&u)
    {
        type(std::move(u)).swap(*this);
        return *this;
    }

    void swap(expected &o)
    {
        using std::swap;
        if (base::has && o.has)
        {
            // Do nothing.
        }
        else if (base::has && !o.has)
        {
            error_type e(std::move(o.s.err));
            ::new (std::addressof(base::s.err)) error_type(e);
            swap(base::has, o.has);
        }
        else if (!base::has && o.has)
        {
            ::new (std::addressof(o.s.err)) error_type(std::move(base::s.err));
            swap(base::has, o.has);
        }
        else
            swap(base::s.err, o.s.err);
    }

    constexpr explicit operator bool() const
    {
        return base::has;
    }

    constexpr bool has_value() const
    {
        return base::has;
    }

    void value() const
    {
        !base::has ? throw bad_expected_access<void>() : void();
    }

    constexpr const E &error() const &
    {
        if (base::has)
            throw bad_expected_access<void>();
        return base::s.err;
    }

    E &error() &
    {
        if (base::has)
            throw bad_expected_access<void>();
        return base::s.err;
    }

    constexpr E &&error() &&
    {
        if (base::has)
            throw bad_expected_access<void>();
        return std::move(base::s.err);
    }
};

template <class T, class E>
constexpr bool operator==(const expected<T, E> &x, const expected<T, E> &y)
{
    return bool(x) == bool(y) && (x ? x.value() == y.value() : x.error() == y.error());
}

template <class T, class E>
constexpr bool operator!=(const expected<T, E> &x, const expected<T, E> &y)
{
    return !(x == y);
}

template <class E>
constexpr bool operator==(const expected<void, E> &x, const expected<void, E> &y)
{
    return bool(x) == bool(y) && (x ? true : x.error() == y.error());
}

template <class T, class E>
constexpr bool operator==(const expected<T, E> &x, const T &y)
{
    return x ? *x == y : false;
}

template <class T, class E>
constexpr bool operator==(const T &x, const expected<T, E> &y)
{
    return y ? x == *y : false;
}

template <class T, class E>
constexpr bool operator!=(const expected<T, E> &x, const T &y)
{
    return x ? *x != y : true;
}

template <class T, class E>
constexpr bool operator!=(const T &x, const expected<T, E> &y)
{
    return y ? x != *y : true;
}

template <class T, class E>
constexpr bool operator==(const expected<T, E> &x, const unexpected<E> &y)
{
    return x ? false : x.error() == y.value();
}

template <class T, class E>
constexpr bool operator==(const unexpected<E> &x, const expected<T, E> &y)
{
    return y ? false : x.value() == y.error();
}

template <class T, class E>
constexpr bool operator!=(const expected<T, E> &x, const unexpected<E> &y)
{
    return x ? true : x.error() != y.value();
}

template <class T, class E>
constexpr bool operator!=(const unexpected<E> &x, const expected<T, E> &y)
{
    return y ? true : x.value() != y.error();
}

template <typename T, typename E>
void swap(expected<T, E> &x, expected<T, E> &y)
{
    x.swap(y);
};
} // namespace core
} // namespace kr

inline constexpr auto &unexpect = kr::core::unexpect;

template <class T, class E>
using expected = kr::core::expected<T, E>;
