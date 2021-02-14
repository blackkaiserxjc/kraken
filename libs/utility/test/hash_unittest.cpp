#include <boost/test/unit_test.hpp>

#include <kr/utility/hash.h>
#include <kr/utility/murmurhash.h>

BOOST_AUTO_TEST_SUITE(Hash)

BOOST_AUTO_TEST_CASE(basic_test)
{
    BOOST_CHECK_EQUAL(kr::utility::hash_str("42"), 0x3224B088);
    BOOST_CHECK_EQUAL(kr::utility::hash_str("hello world"), 0x0D4A1185);
}

BOOST_AUTO_TEST_CASE(murmur_hash_test)
{
    std::string text = "42";
    BOOST_CHECK_EQUAL(MurmurHash(text.data(), static_cast<int>(text.size()), 0), kr::utility::murmur_hash{}(text));
}

BOOST_AUTO_TEST_SUITE_END()
