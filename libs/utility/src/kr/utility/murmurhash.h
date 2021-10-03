#pragma once

#include <cstdint>
#include <string_view>

#if defined(__x86_64__)
std::uint64_t MurmurHash64(const void *key, int len, std::uint32_t seed);
#define MurmurHash MurmurHash64
#elif defined(__i386__)
std::uint32_t MurmurHash2(const void *key, int len, std::uint32_t seed);
#define MurmurHash MurmurHashNeutral2
#else
std::uint32_t MurmurHashNeutral2(const void *key, int len, std::uint32_t seed);
#define MurmurHash MurmurHashNeutral2
#endif

namespace kr {
namespace utility {

struct murmur_hash
{
    std::size_t operator()(std::string_view slice) const
    {
        return MurmurHash(slice.data(), static_cast<int>(slice.size()), 0);
    }
};

} // namespace utility
} // namespace kr
