//
// Created by kaiser on 2020/12/27.
//

#ifndef KR_COMMON_CORE_LANG_H_
#define KR_COMMON_CORE_LANG_H_

#ifndef FALLTHROUGH_INTENDED
#if defined(__clang__)
#define FALLTHROUGH_INTENDED [[clang::fallthrough]]
#elif defined(__GNUC__) && __GNUC__ >= 7
#define FALLTHROUGH_INTENDED [[gnu::fallthrough]]
#else
#define FALLTHROUGH_INTENDED \
    do                       \
    {                        \
    } while (0)
#endif
#endif

#endif // KR_COMMON_CORE_LANG_H_