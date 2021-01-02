find_package(Boost REQUIRED )

include_directories(${Boost_INCLUDE_DIRS})

set(kr_common_logging_SRCS
    logger.h
    logger.cpp
)

add_library(kr_common_logging ${kr_common_logging_SRCS})
set_target_properties(kr_common_logging PROPERTIES FOLDER CommonTargets/Logging)
