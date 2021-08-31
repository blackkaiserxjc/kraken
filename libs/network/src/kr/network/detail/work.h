#ifndef KR_COMMON_NETWORK_DETAIL_WORK_H_
#define KR_COMMON_NETWORK_DETAIL_WORK_H_

#include <boost/asio/executor_work_guard.hpp>

namespace kr {
namespace network {
namespace detail {

template <typename Handler>
class work_dispatcher
{
public:
private:
    Handler handler_;
    boost::asio::executor_work_guard work_;
};

class work_poster
{
};

} // namespace detail
} // namespace network
} // namespace kr

#endif