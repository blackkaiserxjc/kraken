//
// Created by alien on 2021/2/14.
//

#ifndef KR_COMMON_UTILITY_RANDOM_H_
#define KR_COMMON_UTILITY_RANDOM_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <cstdint>
#include <random>

namespace kr {
namespace utility {

/** 32位随机数 **/
class random
{
public:
    /**
     * 构造函数
     * @param seed 随机种子
     */
    explicit random(std::uint32_t seed);

    /**
     * 设置随机种子
     * @param new_seed 随机种子
     */
    void seed(std::uint32_t new_seed);

    /**
     * 生成下一个随机数
     * @return 随机数 
     */
    std::uint32_t next();

    /**
     * 生成一个整数随机数,随机数范围[min, max]
     * @param min 最小值
     * @param max 最大值
     * @return 随机数 
     */
    std::uint32_t randint(std::uint32_t min, std::uint32_t max);

    /**
     * 生成一个实数随机数,随机数范围[min, max]
     * @param min 最小值
     * @param max 最大值
     * @return 随机数
     */
    float uniform(float min, float max);

private:
    std::mt19937 generator_;
};

/** 64位随机数 **/
class random64
{
public:
    /**
     * 构造函数
     * @param seed 随机种子
     */
    explicit random64(std::uint64_t seed);

    /**
     * 设置随机种子
     * @param new_seed 随机种子
     */
    void seed(std::uint64_t new_seed);

    /**
     * 生成下一个随机数
     * @return 随机数 
     */
    std::uint64_t next();

    /**
     * 生成一个整数随机数,随机数范围[min, max]
     * @param min 最小值
     * @param max 最大值
     * @return 随机数 
     */
    std::uint64_t randint64(std::uint64_t min, std::uint64_t max);

    /**
     * 生成一个实数随机数,随机数范围[min, max]
     * @param min 最小值
     * @param max 最大值
     * @return 随机数
     */
    double uniform(double min, double max);

private:
    std::mt19937_64 generator_;
};

}
}


#endif//KR_COMMON_UTILITY_RANDOM_H_

