
#include <kr/utility/string.h>

namespace kr {
namespace utility {
namespace detail {
std::size_t hex_dump_line(const void *ptr, std::size_t offset, std::size_t size, std::string &line);
} // namespace detail

template <class OutIter>
void hex_dump(const void *ptr, std::size_t size, OutIter out)
{
    std::size_t offset = 0;
    std::string line;
    while (offset < size)
    {
        offset += detail::hex_dump_line(ptr, offset, size, line);
        *out++ = line;
    }
}
} // namespace utility
} // namespace kr