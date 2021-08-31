find_package(Protobuf REQUIRED)

include_directories(${PROTOBUF_INCLUDE_DIRS})

set(kr_common_rpc_PROTOS
    options.proto
)

kr_protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${kr_common_rpc_PROTOS})

set(kr_common_rpc_SRCS
    ${PROTO_SRCS}
    ${PROTO_HDRS}
    channel.h
    channel.cpp
)

add_library(kr_common_rpc
    ${kr_common_rpc_SRCS}
)

set_target_properties(kr_common_rpc PROPERTIES FOLDER CommonTargets/Rpc)
