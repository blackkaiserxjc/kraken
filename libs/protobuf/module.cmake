file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/CMakeCppFiles)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/CMakeProtoFiles)

include_directories(${CMAKE_CURRENT_LIST_DIR}/src)
include_directories(${CMAKE_CURRENT_LIST_DIR}/CMakeCppFiles)

macro(kr_protobuf_generate_cpp SRCS HDRS)
    set(${SRCS})
    set(${HDRS})
    foreach(FIL ${ARGN})
        get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
        get_filename_component(FIL_WE ${FIL} NAME_WE)
        list(APPEND ${SRCS} "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.cc")
        list(APPEND ${HDRS} "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.h")
        add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.cc" "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.h"
            COMMAND protobuf::protoc -I=${PROTOBUF_INCLUDE_DIRS} -I=${CMAKE_BINARY_DIR}/CMakeProtoFiles -I=${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=${CMAKE_CURRENT_BINARY_DIR} ${ABS_FIL}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}            
            DEPENDS ${ABS_FIL} protobuf::protoc
            COMMENT "Running cpp protocol buffer compiler on ${FIL_WE}"
            VERBATIM)
    endforeach()
    set(${HDRS} ${${HDRS}} PARENT_SCOPE)
    set(${SRCS} ${${SRCS}} PARENT_SCOPE)
endmacro(kr_protobuf_generate_cpp)