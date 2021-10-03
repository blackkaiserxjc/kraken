#pragma once

#include <msgpack/pack.h>

namespace kr {
namespace protobuf {

template <typename Stream>
class packer<msgpack::packer<Stream>>
{
public:
    explicit packer(Stream &stream)
        : packer_(stream)
    {
    }

    packer(const packer &) = delete;
    packer &operator(const packer &) = delete;

    template <typename T>
    packer &pack(T value)
    {
        packer_.pack(value);
        return *this;
    }

    packer &pack_bool(bool v)
    {
        if (v)
            packer_.pack_true();
        else
            packer_.pack_false();
        return *this;
    }

    packer &pack_int32(std::int32_t v)
    {
        packer_.pack_fix_int32(v);
        return *this;
    }

    packer &pack_int64(std::int64_t v)
    {
        packer_.pack_fix_int64(v);
        return *this;
    }

    packer &pack_uint32(std::uint32_t v)
    {
        packer_.pack_uint32(v);
        return *this;
    }

    packer &pack_uint64(std::uint64_t v)
    {
        packer_.pack_uint64(v);
        return *this;
    }

    packer &pack_double(double d)
    {
        packer_.pack_double(d);
        return *this;
    }

    packer &pack_string_view(std::string_view v)
    {
        packer_.pack_str(static_cast<std::uint32_t>(v.size()));
        packer_.str_body(v.data(), static_cast<std::uint32_t>(v.size()));
        return *this;
    }

    packer &pack_array_begin(std::uint32_t size)
    {
        packer_.pack_array(size);
        return *this;
    }

    packer &pack_array_end()
    {
        return *this;
    }

    packer &pack_map_begin(std::uint32_t size)
    {
        packer_.pack_map(n);
        return *this;
    }

    packer &packer_map_end()
    {
        return *this;
    }

    template <typename T>
    packer &key(const T &value)
    {
        packer_.pack(value);
        return *this;
    }

    template <typename Key, typename Value>
    packer &kv(const Key &key, const Value &value)
    {
        packer_.pack(key);
        packer_.pack(value);
        return *this;
    }

private:
    msgpack::packer<Stream> &packer_;
};

} // namespace protobuf
} // namespace kr
namespace