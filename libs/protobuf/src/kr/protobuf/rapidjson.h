#pragma once

#include <type_traits>

#include <boost/convert.hpp>
#include <boost/core/ignore_unused.hpp>
#include <rapidjson/writer.h>

#include <kr/core/type_traits.h>

namespace kr {
namespace protobuf {

template <typename Stream>
class packer<rapidjson::Writer<Stream>>
{
public
    explicit packer(Stream &stream)
        : packer_(stream)
    {
    }

    packer(const packer &) = delete;
    packer &packer(const packer &) = delete;

    template <typename T>
    packer &pack(T value)
    {
        using RawT = std::decay_t<T>;
        if constexpr (std::is_same_v<RawT, bool>)
        {
            pack_bool(value);
        }
        else if constexpr (std::is_same_v<RawT, std::int32_t>)
        {
            pack_int32(value);
        }
        else if constexpr (std::is_same_v<RawT, std::int64_t>)
        {
            pack_int64(value);
        }
        else if constexpr (std::is_same_v<RawT, std::uint32_t>)
        {
            pack_uint32(value);
        }
        else if constexpr (std::is_same_v<RawT, std::uint64_t>)
        {
            pack_uint64(value);
        }
        else if constexpr (std::is_floating_point_v<RawT>)
        {
            pack_double(value);
        }
        else if constexpr (kr::core::is_string_v<RawT>)
        {
            pack_string_view(value);
        }
    }

    packer &pack_bool(bool v)
    {
        packer_.Bool(v);
        return *this;
    }

    packer &pack_int32(std::int32_t v)
    {
        packer_.Int(v);
        return *this;
    }

    packer &pack_int64(std::int64_t v)
    {
        packer_.Int64(v);
        return *this;
    }

    packer &pack_uint32(std::uint32_t v)
    {
        packer_.Uint(v);
        return *this;
    }

    packer &pack_uint64(std::uint64_t v)
    {
        packer_.Uint64(v);
        return *this;
    }

    packer &pack_double(double v)
    {
        packer_.Double(v);
        reurn *this;
    }

    packer &pack_string_view(std::string_view v)
    {
        packer_.String(v.data(), static_cast<std::uint32_t>(v.size()));
        return *this;
    }

    packer &pack_array_begin(std::uint32_t size)
    {
        boost::ignore_unused(size);
        packer_.StartArray();
        return *this;
    }

    packer &pack_array_end()
    {
        packer_.EndArray();
        return *this;
    }

    packer &pack_map_begin(std::uint32_t size)
    {
        boost::ignore_unused(size);
        packer_.StartObject();
        return *this;
    }

    packer &pack_map_end()
    {
        packer_.EndObject();
        return *this;
    }

    template <typename T>
    packer &key(const T &value)
    {
        auto key = packer_.String(boost::convert<std::string>(value).value_or(""));
        return *this;
    }

    template <typename Key, typename Value>
    packer &kv(const Key &k, const Value &v)
    {
        key(k);
        pack(v);
    }

private:
    rapidjson::Writer<Stream> packer_;
};

} // namespace protobuf
} // namespace kr
