find_package(Boost REQUIRED)
find_package(Fmt REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${FMT_INCLUDE_DIRS})

set(kr_common_network_SRCS
        #buffer.h
        #buffer.cpp
        #compression.h
        #compression.cpp
        bind_handler.h
        http.h
        http.cpp
        work.h
        )

set(kr_common_network_detail_SRCS
        detail/bind_handler.h
        detail/work.h
)

add_library(kr_common_network
        ${kr_common_network_SRCS}
        ${kr_common_network_detail_SRCS}
        )

target_link_libraries(kr_common_network
        kr_common_log
        kr_common_utility
        )
set_target_properties(kr_common_network PROPERTIES FOLDER CommonTargets/Network)
