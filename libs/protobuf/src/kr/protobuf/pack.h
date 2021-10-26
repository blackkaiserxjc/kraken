//
// Created by kaiser on 2021/9/24.
//

#pragma once

#include <kr/core/type.h>

namespace kr {
namespace protobuf {

// write handler concept
template <typename Stream>
class packer
{
public:
    template <typename T>
    packer &pack(T value);

    packer &pack_bool(bool v);
    packer &pack_int32(std::int32_t v);
    packer &pack_int64(std::int64_t v);
    packer &pack_uint32(std::uint32_t v);
    packer &pack_uint64(std::uint64_t v);
    packer &pack_double(double d);
    packer &pack_string_view(std::string_view v);

    packer &pack_array_begin(std::uint32_t size);
    packer &pack_array_end(std::uint32_t count = 0);

    packer &pack_map_begin(std::uint32_t size);
    packer &pack_map_end(std::uint32_t count = 0);

    template <typename T>
    packer &key(const T &value);

    template <typename Key, typename Value>
    packer &kv(const Key &key, const Value &value);
};

} // namespace protobuf
} // namespace kr
