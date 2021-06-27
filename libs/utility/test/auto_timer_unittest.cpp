#include <boost/test/unit_test.hpp>
#include <kr/utility/auto_timer.h>

BOOST_AUTO_TEST_SUITE(AutoTimer)

BOOST_AUTO_TEST_CASE(Basic)
{
    auto timer = kr::utility::make_auto_timer("auto timer");
    timer.log("io test begin.");
    timer.log_format("{} log message", "third");
}

BOOST_AUTO_TEST_SUITE_END()