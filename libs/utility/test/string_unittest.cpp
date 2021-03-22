#include <string>
#include <boost/test/unit_test.hpp>

#include <kr/utility/string.h>
using namespace kr::utility;

BOOST_AUTO_TEST_SUITE(String)
BOOST_AUTO_TEST_SUITE(PrettyPrint)

struct PrettyTestCase
{
    std::string pretty_string;
    double  real_value;
    PrettyType pretty_type;
};

PrettyTestCase pretty_test_cases[] =
{
    {std::string("853 ms"), 85.3e-2, PRETTY_TIME_HMS},
    {std::string("8.53 s "), 85.3e-1, PRETTY_TIME_HMS},
    {std::string("1.422 m "), 85.3, PRETTY_TIME_HMS},
    {std::string("14.22 m "), 85.3e1, PRETTY_TIME_HMS},
    {std::string("2.369 h "), 85.3e2, PRETTY_TIME_HMS},
    {std::string("2.369e+04 h "), 85.3e6, PRETTY_TIME_HMS},

    {std::string("8.53e+07 s "), 85.3e6, PRETTY_TIME},
    {std::string("8.53e+07 s "), 85.3e6, PRETTY_TIME},
    {std::string("85.3 ms"), 85.3e-3, PRETTY_TIME},
    {std::string("85.3 us"), 85.3e-6, PRETTY_TIME},
    {std::string("85.3 ns"), 85.3e-9, PRETTY_TIME},
    {std::string("85.3 ps"), 85.3e-12, PRETTY_TIME},
    {std::string("8.53e-14 s "), 85.3e-15, PRETTY_TIME},

    {std::string("endoftest"), 0, PRETTY_NUM_TYPES},
};

BOOST_AUTO_TEST_CASE(Basic)
{
    for (auto index = 0; pretty_test_cases[index].pretty_type != PRETTY_NUM_TYPES; ++index)
    {
        auto&& pretty_test_case = pretty_test_cases[index];
        BOOST_CHECK_EQUAL(pretty_test_case.pretty_string,
                          pretty_print(pretty_test_case.real_value, pretty_test_case.pretty_type));
    }
}

BOOST_AUTO_TEST_CASE(HexDump)
{
    std::string a("abc\x00\x02\xa0", 6);
    BOOST_CHECK_EQUAL(
        "00000000  61 62 63 00 02 a0                                 "
        "|abc...          |\n",
        hex_dump(a.data(), a.size()));

    a = "abcdefghijklmnopqrstuvwxyz";
    BOOST_CHECK_EQUAL(
            "00000000  61 62 63 64 65 66 67 68  69 6a 6b 6c 6d 6e 6f 70  "
            "|abcdefghijklmnop|\n"
            "00000010  71 72 73 74 75 76 77 78  79 7a                    "
            "|qrstuvwxyz      |\n",
            hex_dump(a.data(), a.size()));
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()