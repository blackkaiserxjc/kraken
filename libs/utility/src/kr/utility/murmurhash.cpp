#include "murmurhash.h"

#include <kr/core/lang.h>

#if defined(__x86_64__)

std::uint64_t MurmurHash64(const void * key, int len, std::uint32_t seed)
{
    const std::uint64_t m = 0xc6a4a7935bd1e995;
    const int r = 47;

    std::uint64_t h = seed ^ (len * m);

    const std::uint64_t * data = (const std::uint64_t *)key;
    const std::uint64_t * end = data + (len/8);

    while(data != end)
    {
        std::uint64_t k = *data++;

        k *= m;
        k ^= k >> r;
        k *= m;

        h ^= k;
        h *= m;
    }

    const unsigned char * data2 = (const unsigned char*)data;

    switch(len & 7)
    {
    case 7: h ^= ((std::uint64_t)data2[6]) << 48; FALLTHROUGH_INTENDED;
    case 6: h ^= ((std::uint64_t)data2[5]) << 40; FALLTHROUGH_INTENDED;
    case 5: h ^= ((std::uint64_t)data2[4]) << 32; FALLTHROUGH_INTENDED;
    case 4: h ^= ((std::uint64_t)data2[3]) << 24; FALLTHROUGH_INTENDED;
    case 3: h ^= ((std::uint64_t)data2[2]) << 16; FALLTHROUGH_INTENDED;
    case 2: h ^= ((std::uint64_t)data2[1]) << 8;  FALLTHROUGH_INTENDED;
    case 1: h ^= ((std::uint64_t)data2[0]);
        h *= m;
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}

#elif defined(__i386__)

std::uint32_t MurmurHash2(const void *key, int len, std::uint32_t seed)
{
    const std::uint32_t m = 0x5bd1e995;
    const int r = 24;

    std::uint32_t h = seed ^ len;

    const unsigned char * data = (const unsigned char *)key;

    while(len >= 4)
    {
       std::uint32_t k = *(std::uint32_t *)data;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch(len)
    {
    case 3: h ^= data[2] << 16; FALLTHROUGH_INTENDED;
    case 2: h ^= data[1] << 8;  FALLTHROUGH_INTENDED;
    case 1: h ^= data[0];
        h *= m;
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}

#else

std::uint32_t MurmurHashNeutral2(const void *key, int len, std::uint32_t seed)
{   
    const std::uint32_t m = 0x5bd1e995;
    const int r = 24;

    std::uint32_t h = seed ^ len;

    const unsigned char * data = (const unsigned char *)key;

    while(len >= 4)
    {
        std::uint32_t k;

        k  = data[0];
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch(len)
    {
    case 3: h ^= data[2] << 16; FALLTHROUGH_INTENDED;
    case 2: h ^= data[1] << 8;  FALLTHROUGH_INTENDED;
    case 1: h ^= data[0];
        h *= m;
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}

#endif