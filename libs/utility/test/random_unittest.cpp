#include <algorithm>
#include <vector>
#include <map>

#include <boost/test/unit_test.hpp>
#include <fmt/format.h>

#include <kr/utility/random.h>

BOOST_AUTO_TEST_SUITE(RandomTest)

BOOST_AUTO_TEST_CASE(rand_int_test)
{
    using kr::utility::random;
    const int average = 20;
    for(std::uint32_t seed : {0, 1, 2, 37, 4096})
    {
        random r{seed};
        for (auto range : {1, 2, 8, 12, 100})
        {
            std::vector<int> counts(range, 0);
            for (int i = 0; i < range * average; ++i)
            {
                auto value = r.randint(0, range - 1);
                ++counts.at(value);
            }
            int max_variance = static_cast<int>(std::sqrt(range) * 2 + 4);
            for (int i = 0; i < range; ++i)
            {
                BOOST_CHECK_GE(counts[i], std::max(1, average - max_variance));
                BOOST_CHECK_LE(counts[i], average + max_variance + 1);
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(uniform_test)
{
    using kr::utility::random;
    std::map<float, float> ranges = {
        {0.0, 1.2},
        {3.0, 100.1},
        {-42, 42},
        {-110.0, 0.0}
    };
    for (std::uint32_t seed : {0, 1, 2, 37, 4096})
    {
        random r{seed};
        for(auto[min, max] : ranges)
        {
            auto value = r.uniform(min, max);
            BOOST_CHECK_GE(value, min);
            BOOST_CHECK_LE(value, max);
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()