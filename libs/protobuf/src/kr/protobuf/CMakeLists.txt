find_package(Protobuf REQUIRED)
find_package(Boost REQUIRED COMPONENTS)

set(kr_common_protobuf_SRCS
    service.h
)

set(kr_common_protobuf_SRCS
    service.h
)

add_library(kr_common_protobuf 
    INTERFACE
    ${kr_common_protobuf_SRCS}
)

set_target_properties(kr_common_protobuf PROPERTIES FOLDER CommonTargets/Protobuf)
