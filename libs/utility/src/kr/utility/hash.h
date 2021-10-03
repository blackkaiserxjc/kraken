//
// Created by kaiser on 2021/1/1.
//
#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>

#include <boost/crc.hpp>

namespace kr {
namespace utility {

/**
 *  计算字符串的哈希值
 * @param sv 字符串
 * @return   哈希值
 */
inline std::uint32_t hash_str(std::string_view sv)
{
    boost::crc_32_type result;
    result.process_bytes(sv.data(), sv.length());
    return result.checksum();
}

} // namespace utility
} // namespace kr
