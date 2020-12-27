find_package(Boost REQUIRED system filesystem unit_test_framework)

include_directories(${Boost_INCLUDE_DIRS})

set(kr_common_utility_test_SRCS
    auto_timer_unittest.cpp
    unittest_main.cpp
)

add_executable(kr_common_utility_test ${kr_common_utility_test_SRCS})
target_link_libraries(kr_common_utility_test
    #kr_common_utility
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
add_test(NAME kr_common_utility_test COMMAND kr_common_utility_test)

set_target_properties(kr_common_utility_test PROPERTIES FOLDER CommonTargets/Utility)