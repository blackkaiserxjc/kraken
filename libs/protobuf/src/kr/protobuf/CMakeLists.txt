find_package(Boost REQUIRED COMPONENTS)
find_package(Protobuf REQUIRED)
find_package(msgpack CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)

set(kr_common_protobuf_SRCS
    service.h
    rapidjson.h
    msgpack.h
    pack.h
    unpack.h
)

add_library(kr_common_protobuf 
    INTERFACE
    ${kr_common_protobuf_SRCS}
)

set_target_properties(kr_common_protobuf PROPERTIES FOLDER CommonTargets/Protobuf)
