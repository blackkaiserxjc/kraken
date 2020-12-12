# - Find Fmt
# Find the fmt includes and library
#
# FMT_INCLUDE_DIR, where to find fmt headers
# FMT_LIBRARY, fmtlib libraries
# FMT_FOUND, If false, do not try to use fmt

find_path(FMT_INCLUDE_DIR NAMES fmt/core.h)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    find_library(FMT_LIBRARIES NAMES fmtd)
else()
    find_library(FMT_LIBRARIES NAMES fmt)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(fmt DEFAULT_MSG FMT_INCLUDE_DIRS FMT_LIBRARIES)

mark_as_advanced(FMT_INCLUDE_DIRS FMT_LIBRARIES)



