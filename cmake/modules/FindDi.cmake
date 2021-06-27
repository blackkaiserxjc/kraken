# - Find boost-di library
# Find the sol includes
#
# BOOST_DI_INCLUDE_DIRS - where to find boost/di.hpp, etc.
# BOOST_DI_FOUND - True if sol found.

find_path(BOOST_DI_INCLUDE_DIRS NAMES boost/di.hpp)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Di DEFAULT_MSG BOOST_DI_INCLUDE_DIRS)

mark_as_advanced(BOOST_DI_INCLUDE_DIRS)


