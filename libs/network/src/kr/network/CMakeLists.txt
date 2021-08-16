find_package(Boost REQUIRED)
find_package(Fmt REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${FMT_INCLUDE_DIRS})

set(kr_common_net_SRCS
    buffer.h
    buffer.cpp
    compression.h
    compression.cpp
)

add_library(kr_common_net
    ${kr_common_net_SRCS}
)

target_link_libraries(kr_common_net
    kr_common_log
    kr_common_utility
)
set_target_properties(kr_common_net PROPERTIES FOLDER CommonTargets/Network)
