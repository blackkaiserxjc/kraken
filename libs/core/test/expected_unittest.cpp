#include <boost/test/unit_test.hpp>
#include <kr/core/expected.h>

#include <string>

enum class E
{
    E1,
    E2
};

BOOST_AUTO_TEST_SUITE(Expected)

BOOST_AUTO_TEST_CASE(Simple)
{
    expected<int, int> ex;
    BOOST_TEST(!bool(ex));
    BOOST_CHECK_EQUAL(42, ex.value_or(42));
    BOOST_TEST(bool(ex));

    ex = kr::core::make_unexpected(-1);
    BOOST_TEST(!bool(ex));
    BOOST_CHECK_EQUAL(-1, ex.error());
    BOOST_CHECK_EQUAL(42, ex.value_or(42));
}

BOOST_AUTO_TEST_CASE(SwapMethod)
{
    expected<std::string, int> a;
    expected<std::string, int> b;

    a.swap(b);
    BOOST_TEST(!a.has_value());
    BOOST_TEST(!b.has_value());

    a = "hello";
    BOOST_TEST(a.has_value());
    BOOST_TEST(!b.has_value());
}

BOOST_AUTO_TEST_SUITE_END()