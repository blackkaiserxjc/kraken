# - Find Fmt
# Find the fmt includes and library
#
# FMT_INCLUDE_DIRS, where to find fmt headers
# FMT_LIBRARY, fmtlib libraries
# FMT_FOUND, If false, do not try to use fmt

find_path(FMT_INCLUDE_DIRS NAMES fmt/format.h)
find_library(FMT_LIBRARIES NAMES fmt fmtd)
mark_as_advanced(FMT_INCLUDE_DIRS FMT_LIBRARIES)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Fmt
        DEFAULT_MSG
        FMT_INCLUDE_DIRS FMT_LIBRARIES)



