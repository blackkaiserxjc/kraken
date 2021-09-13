find_package(Boost REQUIRED)
find_package(Protobuf REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${Protobuf_INCLUDE_DIRS})

set(kr_proto_lua_plugin_SRCS
    lua_generator.h
    lua_generator.cpp
    lua_plugin.h
    lua_plugin.cpp
)

add_library(kr_proto_lua_plugin ${kr_proto_lua_plugin_SRCS})
target_link_libraries(kr_proto_lua_plugin
    ${Protobuf_LIBRARIES}
)

set_target_properties(kr_proto_lua_plugin PROPERTIES FOLDER CommonTargets/Protobuf)