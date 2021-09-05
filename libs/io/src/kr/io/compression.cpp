//
// Created by kaiser on 2021/1/2.
//

#include "compression.h"

#include <fmt/format.h>
#include <stdexcept>

namespace kr {
namespace utility {
compression::compression(CompressType type, std::optional<int> level)
    : type_(type)
{
}

std::string compression::compress(std::string_view data)
{
    return {};
}

std::string compression::decompress(std::string_view data)
{
    return {};
}

class gzip_compression : public compression
{
public:
    static std::unique_ptr<compression> create(int level, CompressType type);

    explicit gzip_compression(int level, CompressType type);

private:
};

class lzma_compression : public compression
{
public:
    static std::unique_ptr<compression> create(int level, CompressType type);

    explicit lzma_compression(int level, CompressType type);

private:
};

class bzip2_compression : public compression
{
public:
    static std::unique_ptr<compression> create(int level, CompressType type);

    explicit bzip2_compression(int level, CompressType type);

private:
};

class zstd_compression : public compression
{
public:
    static std::unique_ptr<compression> create(int level, CompressType type);

    explicit zstd_compression(int level, CompressType type);

private:
};

class zlib_compression : public compression
{
public:
    static std::unique_ptr<compression> create(int level, CompressType type);

    explicit zlib_compression(int level, CompressType type);

private:
};

using codec_factory = std::function<std::unique_ptr<compression>(int, CompressType)>;
struct factory
{
    codec_factory codec;
};
constexpr factory codec_factorys[static_cast<size_t>(CompressType::NUM_TYPES)] =
    {
        {},
        {gzip_compression::create},
        {lzma_compression::create},
        {bzip2_compression::create},
        {zstd_compression::create},
        {zlib_compression::create},
};

factory const &get_factory(CompressType type)
{
    auto const idx = static_cast<size_t>(type);
    if (idx >= static_cast<size_t>(CompressType::NUM_TYPES))
    {
        throw std::invalid_argument(fmt::format("compression type {} invaild.", idx));
    }
    return codec_factorys[idx];
}

std::unique_ptr<compression> make_compression(CompressType type, int level)
{
    auto const factory = get_factory(type).codec;
    if (!factory)
    {
        throw std::invalid_argument(fmt::format("compression type {} not supported.",
            static_cast<size_t>(type)));
    }
    auto codec = (*factory)(level, type);
    return codec;
}
} // namespace utility
} // namespace kr