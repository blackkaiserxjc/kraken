//
// Created by alien on 2021/2/14.
//

#include "random.h"

namespace kr {
namespace utility {

random::random(std::uint32_t seed)
    :generator_{seed}
{
}

void random::seed(std::uint32_t new_seed)
{
    generator_.seed(new_seed);
}

std::uint32_t random::next() 
{
    return static_cast<std::uint32_t>(generator_());
}

std::uint32_t random::randint(std::uint32_t min, std::uint32_t max)
{
    return static_cast<std::uint32_t>(std::uniform_int_distribution<std::mt19937::result_type>(min, max)(generator_));
}

float random::uniform(float min, float max)
{
    return static_cast<float>(std::uniform_real_distribution<float>(min, max)(generator_));
}

random64::random64(std::uint64_t seed)
    :generator_{seed}
{
}

void random64::seed(std::uint64_t new_seed)
{
    generator_.seed(new_seed);
}

std::uint64_t random64::next()
{
    return static_cast<std::uint64_t>(generator_());
}

std::uint64_t random64::randint64(std::uint64_t min, std::uint64_t max)
{
    return static_cast<std::uint64_t>(std::uniform_int_distribution<std::uint64_t>(min, max)(generator_));
}

double random64::uniform(double min, double max)
{
    return static_cast<double>(std::uniform_real_distribution<>(min, max)(generator_));
}

}
}
