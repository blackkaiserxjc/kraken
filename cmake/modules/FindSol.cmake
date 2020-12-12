# - Find sol library
# Find the sol includes
#
# SOL_INCLUDE_DIRS - where to find sol.hpp, etc.
# SOL_FOUND - True if sol found.

find_path(SOL_INCLUDE_DIRS NAMES sol.hpp)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(sol DEFAULT_MSG SOL_INCLUDE_DIRS)

mark_as_advanced(SOL_INCLUDE_DIRS)
