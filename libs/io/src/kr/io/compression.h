//
// Created by kaiser on 2021/1/2.
//

#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace kr {
namespace io {

/** 压缩类型 **/
enum class CompressType
{
    NONE = 0,
    GZIP = 1,
    LZMA = 2,
    BZIP2 = 3,
    ZSTD = 4,
    ZLIB = 5,

    NUM_TYPES = 6,
};

class compression
{
public:
    /**
     *  获取压缩算法类型
     *  @return 类型
     */
    CompressType type() const
    {
        return type_;
    };

    /**
     * 压缩
     * @param data 数据源
     * @return 压缩后数据
     */
    std::string compress(std::string_view data);

    // TODO: 压缩io buffer相关的接口

    /**
     * 解压
     * @param data 数据源
     * @return 解压后数据
     */
    std::string decompress(std::string_view data);

    // TODO: 解压io buffer相关接口

protected:
    // 构造
    compression(CompressType type, std::optional<int> level = std::nullopt);

private:
    // TODO: 压缩接口
    virtual do_compress(*) = 0;

    // TODO: 解压接口
    virtual do_decompress(*) = 0;

    // 压缩字符串
    virtual std::string do_compress_string(std::string_view data);

    // 解压字符串
    virtual std::string do_decompress_string(std::string_view data);

    // 压缩类型
    CompressType type_;
};

constexpr int COMPRESS_LEVEL_FASTEST = -1;
constexpr int COMPRESS_LEVEL_DEFAULT = -2;
constexpr int COMPRESS_LEVEL_BEST = -3;

/**
 * 返回指定类型的compression
 * @param type  压缩类型
 * @param level 压缩等级
 * @return compression
 */
std::unique_ptr<compression> make_compression(CompressType type, int level = COMPRESS_LEVEL_DEFAULT);

} // namespace io
} // namespace kr
