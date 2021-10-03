#pragma once

#include <boost/asio.hpp>

namespace kr {
namespace rpc {

template <typename... Args>
class channel
{
public:
    inline channel() = default;
    virtual ~channel() = default;
};

class service
{
public:
    inline service() = default;
    virtual ~service() = default;
};

} // namespace rpc
} // namespace kr
