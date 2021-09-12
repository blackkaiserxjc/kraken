# - Find Fmt
# Find the fmt includes and library
#
# FMT_INCLUDE_DIRS, where to find fmt headers
# FMT_LIBRARY, fmtlib libraries
# FMT_FOUND, If false, do not try to use fmt

find_path(FMT_INCLUDE_DIR NAMES fmt/format.h)

if(FMT_INCLUDE_DIR)
  set(_fmt_version_file "${FMT_INCLUDE_DIR}/fmt/core.h")
  if(NOT EXISTS "${_fmt_version_file}")
    set(_fmt_version_file "${FMT_INCLUDE_DIR}/fmt/format.h")
  endif()
  if(EXISTS "${_fmt_version_file}")
    # parse "#define FMT_VERSION 40100" to 4.1.0
    file(STRINGS "${_fmt_version_file}" FMT_VERSION_LINE
      REGEX "^#define[ \t]+FMT_VERSION[ \t]+[0-9]+$")
    string(REGEX REPLACE "^#define[ \t]+FMT_VERSION[ \t]+([0-9]+)$"
      "\\1" FMT_VERSION "${FMT_VERSION_LINE}")
    foreach(ver "FMT_VERSION_PATCH" "FMT_VERSION_MINOR" "FMT_VERSION_MAJOR")
      math(EXPR ${ver} "${FMT_VERSION} % 100")
      math(EXPR FMT_VERSION "(${FMT_VERSION} - ${${ver}}) / 100")
    endforeach()
    set(FMT_VERSION
      "${FMT_VERSION_MAJOR}.${FMT_VERSION_MINOR}.${FMT_VERSION_PATCH}")
  endif()
endif()

find_library(FMT_LIBRARY NAMES fmt)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
        FMT
        REQUIRED_VARS FMT_INCLUDE_DIR FMT_LIBRARY
        VERSION_VAR FMT_VERSION)

mark_as_advanced(
        FMT_INCLUDE_DIR
        FMT_LIBRARY
        FMT_VERSION_MAJOR
        FMT_VERSION_MINOR
        FMT_VERSION_PATCH
        FMT_VERSION_STRING)

if(FMT_FOUND AND NOT (TARGET fmt::fmt))
  add_library(fmt-header-only INTERFACE)
  set_target_properties(fmt-header-only PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${FMT_INCLUDE_DIR}"
    INTERFACE_COMPILE_DEFINITIONS FMT_HEADER_ONLY=1
    INTERFACE_COMPILE_FEATURES cxx_std_11)

  add_library(fmt UNKNOWN IMPORTED GLOBAL)
  set_target_properties(fmt PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${FMT_INCLUDE_DIR}"
    INTERFACE_COMPILE_FEATURES cxx_std_11
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_LOCATION "${FMT_LIBRARY}")

  if(WITH_FMT_HEADER_ONLY)
    add_library(fmt::fmt ALIAS fmt-header-only)
  else()
    add_library(fmt::fmt ALIAS fmt)
  endif()
endif()



