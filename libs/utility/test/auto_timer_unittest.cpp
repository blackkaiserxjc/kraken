#include <iostream>
#include <boost/test/unit_test.hpp>
#include <kr/utility/auto_timer.h>

BOOST_AUTO_TEST_SUITE(AutoTimer)

BOOST_AUTO_TEST_CASE(basic_test)
{
    auto timer = kr::utility::make_auto_timer("auto timer");
    timer.log("first log message.");
    timer.log("second log message");
    timer.log_format("{} log message", "third");
}

BOOST_AUTO_TEST_SUITE_END()