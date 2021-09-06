find_package(Boost REQUIRED atomic context date_time filesystem regex system thread log log_setup)
find_package(cxxopts CONFIG REQUIRED)
find_package(Fmt REQUIRED)
find_package(Lua REQUIRED)
find_package(Sol REQUIRED)
find_package(Protobuf REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${FMT_INCLUDE_DIRS})
include_directories(${LUA_INCLUDE_DIR})
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${SOL_INCLUDE_DIRS})

find_program(LUAC_EXECUTABLE NAMES luac PATH_SUFFIXES lua)

# 配置文件
set(kr_app_server_CONFIGS
        server.app.lua
        server.log.conf)

set(kr_app_server_OUTPUT_CONFIGS)
foreach (config_file ${kr_app_server_CONFIGS})
    list(APPEND kr_app_server_OUTPUT_CONFIGS "${CMAKE_CURRENT_BINARY_DIR}/etc/${config_file}")
    add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/etc/${config_file}"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${config_file}" "${CMAKE_CURRENT_BINARY_DIR}/etc/${config_file}"
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${config_file}
            COMMENT "Generaton Config ${config_file}")
endforeach ()
source_group("Output Files" FILES ${kr_app_server_OUTPUT_CONFIGS})
source_group("Config Files" FILES ${kr_app_server_CONFIGS})

# 源码
set(kr_app_server_SRCS
        main.cpp)

add_executable(kr_app_server
        ${kr_app_server_SRCS}
        ${kr_app_server_CONFIGS}
        ${kr_app_server_OUTPUT_CONFIGS})

target_link_libraries(kr_app_server
        kr_common_core
        kr_common_log
        kr_common_utility
        cxxopts::cxxopts
        ${Boost_LOG_SETUP_LIBRARY}
        ${Boost_LOG_LIBRARY}
        ${Boost_ATOMIC_LIBRARY}
        ${Boost_DATE_TIME_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_REGEX_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${FMT_LIBRARIES}
        ${LUA_LIBRARIES}
        ${Protobuf_LIBRARIES}
        ${CRT_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${CMAKE_DL_LIBS})