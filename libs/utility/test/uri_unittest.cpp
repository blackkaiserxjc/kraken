#include <boost/test/unit_test.hpp>
#include <string_view>

#include <kr/utility/uri.h>
using namespace kr::utility;

BOOST_AUTO_TEST_SUITE(Uri)

BOOST_AUTO_TEST_CASE(Basic)
{
    std::string_view str = "https://user:password@www.contoso.com:80/Home/Index.htm?q1=v1&q2=v2#FragmentName";
    uri uri(str);
    BOOST_CHECK_EQUAL(uri.scheme(), "https");
    BOOST_CHECK_EQUAL(uri.username(), "user");
    BOOST_CHECK_EQUAL(uri.password(), "password");
    BOOST_CHECK_EQUAL(uri.host(), "www.contoso.com");
    BOOST_CHECK_EQUAL(uri.port(), 80);
    BOOST_CHECK_EQUAL(uri.path(), "/Home/Index.htm");
    BOOST_CHECK_EQUAL(uri.fragment(), "FragmentName");
    BOOST_CHECK_EQUAL(uri.query(), "q1=v1&q2=v2");
    BOOST_CHECK_EQUAL(uri.str(), str);
}

BOOST_AUTO_TEST_SUITE_END()