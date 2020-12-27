find_package(Boost REQUIRED atomic context date_time filesystem)
find_package(Fmt REQUIRED)
find_package(Protobuf REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${FMT_INCLUDE_DIRS})
include_directories(${LUA_INCLUDE_DIR})
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${SOL_INCLUDE_DIRS})

find_program(LUAC_EXECUTABLE NAMES luac PATH_SUFFIXES lua)

# -- 源码
set(kr_app_server_SRCS
    main.cpp
    )

add_executable(kr_app_server
    ${kr_app_server_SRCS}
)

target_link_libraries(kr_app_server
    kr_common_core
    ${FMT_LIBRARIES}
    ${LUA_LIBRARIES}
    ${PROTOBUF_LIBRARIES}
    ${CRT_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)