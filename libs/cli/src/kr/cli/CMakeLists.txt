find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

set(kr_common_cli_SRCS
    cli.h
    cli.cpp
    scheduler.h
    scheduler.cpp
)

set(kr_common_cli_detail_SRCS

)

add_library(kr_common_cli ${kr_common_cli_SRCS} ${kr_common_cli_detail_SRCS})
set_target_properties(kr_common_cli PROPERTIES FOLDER CommonTargets/CLI)