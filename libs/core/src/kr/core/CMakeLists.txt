find_package(Boost REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${ZLIB_INCLUDE_DIRS})

set(kr_common_core_SRCS
    config.h
    core.cpp
    core.h
    lang.h
    likely.h
)

add_library(kr_common_core ${kr_common_core_SRCS})
set_target_properties(kr_common_core PROPERTIES FOLDER CommonTargets/Core)