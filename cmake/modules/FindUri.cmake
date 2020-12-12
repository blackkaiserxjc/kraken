# Find network-uri library
# Find the network-uri includes
#
# NETWORK_URI_INCLUDE_DIRS - where to find network/uri.hpp, etc.
# NETWORK_URI_LIBRARIES - where to find network-uri, etc.
# NETWORK_URI_FOUND - True if sol found.

find_path(NETWORK_URI_INCLUDE_DIRS NAMES network/uri.hpp)
find_library(NETWORK_URI_LIBRARIES NAMES network-uri)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(network-uri DEFAULT_MSG NETWORK_URI_INCLUDE_DIRS NETWORK_URI_LIBRARIES)

mark_as_advanced(NETWORK_URI_INCLUDE_DIRS NETWORK_URI_LIBRARIES)
