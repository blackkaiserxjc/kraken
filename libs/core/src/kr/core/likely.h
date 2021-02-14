//
// Created by kaiser on 2020/12/27.
//

#ifndef KR_COMMON_CORE_LIKELY_H_
#define KR_COMMON_CORE_LIKELY_H_

#if defined(__GNUC__) && __GNUC__ >= 4
#define LIKELY(x) (__builtin_expect((x), 1))
#define UNLIKELY(x) (__builtin_expect((x), 0))
#else
#define LIKELY(x) (x)
#define UNLIKELY(x) (x)
#endif

#endif
