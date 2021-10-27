#include <boost/test/unit_test.hpp>
#include <kr/core/expected.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>

enum class E
{
    E1,
    E2
};

BOOST_AUTO_TEST_SUITE(Expected)

BOOST_AUTO_TEST_CASE(Simple)
{
    expected<int, int> ex = make_unexpected(-1);
    BOOST_TEST(!bool(ex));
    BOOST_CHECK_EQUAL(-1, ex.error());
    BOOST_CHECK_EQUAL(42, ex.value_or(42));
}

BOOST_AUTO_TEST_CASE(SwapMethod)
{
    expected<std::string, int> a;
    expected<std::string, int> b;

    a.swap(b);
    BOOST_TEST(a.has_value());
    BOOST_TEST(b.has_value());

    a = "hello";
    BOOST_TEST(a.has_value());
    BOOST_TEST(b.has_value());

    b.swap(a);
    BOOST_TEST(a.has_value());
    BOOST_TEST(b.has_value());

    a = "byte";
    BOOST_TEST(a.has_value());
    BOOST_CHECK_EQUAL("byte", a.value());

    a.swap(b);
    BOOST_CHECK_EQUAL("hello", a.value());
    BOOST_CHECK_EQUAL("byte", b.value());
}

BOOST_AUTO_TEST_CASE(StdSwapMethod)
{
    expected<std::string, E> a;
    expected<std::string, E> b;

    std::swap(a, b);
    BOOST_TEST(a.has_value());
    BOOST_TEST(b.has_value());

    a = "greeting";
    BOOST_TEST(a.has_value());
    BOOST_TEST(b.has_value());
    BOOST_CHECK_EQUAL("greeting", a.value());

    std::swap(a, b);
    BOOST_TEST(a.has_value());
    BOOST_TEST(b.has_value());
    BOOST_CHECK_EQUAL("greeting", b.value());

    a = "goodbyte";
    BOOST_TEST(a.has_value());
    BOOST_CHECK_EQUAL("goodbyte", a.value());

    std::swap(a, b);
    BOOST_CHECK_EQUAL("goodbyte", b.value());
    BOOST_CHECK_EQUAL("greeting", a.value());
}

BOOST_AUTO_TEST_CASE(EmptyConstruct)
{
    expected<int, int> ex = make_unexpected(42);
    BOOST_TEST(!bool(ex));
    expected<int, int> test1(ex);
    BOOST_TEST(!bool(test1));
    expected<int, int> test2(std::move(ex));
    BOOST_TEST(!bool(test2));
    BOOST_CHECK_EQUAL(42, ex.error());
}

BOOST_AUTO_TEST_CASE(Conversions)
{
    expected<bool, E> ex_bool;
    expected<short, E> ex_short;
    expected<int, E> ex_int;
    BOOST_TEST(!bool(std::is_convertible<expected<bool, E> &, bool>::value));
    BOOST_TEST(!bool(std::is_convertible<expected<short, E> &, short>::value));
    BOOST_TEST(!bool(std::is_convertible<expected<int, E> &, int>::value));

    ex_bool = false;
    BOOST_TEST(bool(ex_bool));
    BOOST_TEST(!*ex_bool);
}

BOOST_AUTO_TEST_SUITE_END()