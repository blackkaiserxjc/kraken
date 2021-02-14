find_package(Boost REQUIRED system filesystem unit_test_framework)
find_package(Di REQUIRED)
find_package(Fmt REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${BOOST_DI_INCLUDE_DIRS})
include_directories(${FMT_INCLUDE_DIRS})

set(kr_common_utility_test_SRCS
    auto_timer_unittest.cpp
    di_unittest.cpp
    hash_unittest.cpp
    unittest_main.cpp
)

add_executable(kr_common_utility_test ${kr_common_utility_test_SRCS})
target_link_libraries(kr_common_utility_test
    kr_common_utility
    kr_common_core
    ${FMT_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
add_test(NAME kr_common_utility_test COMMAND kr_common_utility_test)

set_target_properties(kr_common_utility_test PROPERTIES FOLDER CommonTargets/Utility)