find_package(Boost REQUIRED)
find_package(Fmt REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${FMT_INCLUDE_DIRS})

set(kr_common_utility_SRCS
    auto_timer.h
    auto_timer.cpp
    hash.h
    murmurhash.h
    murmurhash.cpp
    random.h
    random.cpp
    string.h
    string.cpp
)

set(kr_common_utility_detail_SRCS
    detail/auto_timer.h
    detail/string.h
)

add_library(kr_common_utility ${kr_common_utility_SRCS} ${kr_common_utility_detail_SRCS})
set_target_properties(kr_common_utility PROPERTIES FOLDER CommonTargets/Utility)
