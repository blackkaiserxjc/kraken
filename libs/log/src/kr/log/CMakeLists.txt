find_package(Boost REQUIRED )

include_directories(${Boost_INCLUDE_DIRS})

set(kr_common_log_SRCS
        logging.h
        logging.cpp
)

set(kr_common_log_detail_SRCS
    detail/logging.h
)

add_library(kr_common_log ${kr_common_log_SRCS} ${kr_common_log_detail_SRCS})
set_target_properties(kr_common_log PROPERTIES FOLDER CommonTargets/Log)
