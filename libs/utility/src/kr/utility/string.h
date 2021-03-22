//
// Created by kaiser on 2021/3/21.
//

#ifndef KR_COMMON_UTILITY_HASH_H_
#define KR_COMMON_UTILITY_HASH_H_

#include <string>

namespace kr{
namespace utility{

/**
 * A pretty-printer for numbers that appends suffixes of units of the
 * given type.  It prints 4 sig-figs of value with the most
 * appropriate unit.
 *
 * If `addSpace' is true, we put a space between the units suffix and
 * the value.
 *
 * Current types are:
 *   PRETTY_TIME         - s, ms, us, ns, etc.
 *   PRETTY_TIME_HMS     - h, m, s, ms, us, ns, etc.
 *   PRETTY_BYTES_METRIC - kB, MB, GB, etc (goes up by 10^3 = 1000 each time)
 *   PRETTY_BYTES        - kB, MB, GB, etc (goes up by 2^10 = 1024 each time)
 *   PRETTY_BYTES_IEC    - KiB, MiB, GiB, etc
 *   PRETTY_UNITS_METRIC - k, M, G, etc (goes up by 10^3 = 1000 each time)
 *   PRETTY_UNITS_BINARY - k, M, G, etc (goes up by 2^10 = 1024 each time)
 *   PRETTY_UNITS_BINARY_IEC - Ki, Mi, Gi, etc
 *   PRETTY_SI           - full SI metric prefixes from yocto to Yotta
 *                         http://en.wikipedia.org/wiki/Metric_prefix
 *
 */
enum PrettyType
{
    PRETTY_TIME,
    PRETTY_TIME_HMS,

    PRETTY_BYTES_METRIC,
    PRETTY_BYTES_BINARY,
    PRETTY_BYTES = PRETTY_BYTES_BINARY,
    PRETTY_BYTES_BINARY_IEC,
    PRETTY_BYTES_IEC = PRETTY_BYTES_BINARY_IEC,

    PRETTY_UNITS_METRIC,
    PRETTY_UNITS_BINARY,
    PRETTY_UNITS_BINARY_IEC,

    PRETTY_SI,
    PRETTY_NUM_TYPES,
};

std::string pretty_print(double val, PrettyType type, bool add_space = true);

template<class OutIter>
void hex_dump(const void* ptr, std::size_t size, OutIter out);

/**
 * @param ptr 数据指针
 * @param size
 * @return hex dump
 */
std::string hex_dump(const void* ptr, std::size_t size);

}// namespace utility
}// namespace kr

#include "detail/string.h"

#endif