find_package(Boost REQUIRED)
find_package(Fmt REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${FMT_INCLUDE_DIRS})

set(kr_common_network_SRCS
        #buffer.h
        #buffer.cpp
        #compression.h
        #compression.cpp
        http.h
        http.cpp
        )

add_library(kr_common_network
        ${kr_common_network_SRCS}
        )

target_link_libraries(kr_common_network
        kr_common_log
        kr_common_utility
        )
set_target_properties(kr_common_network PROPERTIES FOLDER CommonTargets/Network)
