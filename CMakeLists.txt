cmake_minimum_required(VERSION 3.17)
project(kraken)

#默认debug
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# 开启测试
enable_testing()

# 平台相关
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi /EHs /DEBUG /std:c++latest")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /GS /Od /MTd /bigobj")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Gy /MT /O2 -DNDEBUG /bigobj")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO /DEBUG")
    add_definitions(-D_WIN32_WINNT=0x0601 -DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE -D_SCL_SECURE_NO_WARNINGS -D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS -D_CRT_SECURE_NO_WARNINGS -D_DISABLE_EXTENDED_ALIGNED_STORAGE)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG")
endif()

# 静态库
option(CMAKE_Cxx_CREATE_STATIC_LIBRARY "Build static library" ON)

# 编译目标使用分组
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#第三方库模块
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

# 核心库
include(libs/cli/module.cmake)
include(libs/core/module.cmake)
include(libs/log/module.cmake)
include(libs/utility/module.cmake)

add_subdirectory(libs/cli)
add_subdirectory(libs/core)
add_subdirectory(libs/log)
add_subdirectory(libs/utility)

# 服务
add_subdirectory(src)