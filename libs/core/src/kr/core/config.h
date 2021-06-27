//
// Created by kaiser on 2020/12/27.
//

#ifndef KR_COMMON_CORE_CONFIG_H_
#define KR_COMMON_CORE_CONFIG_H_

#if defined(_WIN32) && defined(KR_CORE_USE_DLL)
#if defined(KR_CORE_DLL_EXPORT)
#define KR_API __declspec(dllexport)
#else
#define KR_API __declspec(dllimport)
#endif
#endif

#if !defined(KR_API)
#if defined(__GNUC__) && (__GNUC__ >= 4)
#define KR_API __attribute__((visibility("default")))
#else
#define KR_API
#endif
#endif

#if !defined(KR_API)
#define KR_API
#endif

#endif //KRAKEN_CONFIG_H
