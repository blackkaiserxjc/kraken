find_package(Boost REQUIRED COMPONENTS atomic date_time regex system thread filesystem log_setup log unit_test_framework)
find_package(Fmt REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${FMT_INCLUDE_DIRS})

set(kr_common_utility_test_SRCS
        auto_timer_unittest.cpp
        hash_unittest.cpp
        random_unittest.cpp
        string_unittest.cpp
        uri_unittest.cpp
        unittest_main.cpp
        ioc_unittest.cpp)

add_executable(kr_common_utility_test ${kr_common_utility_test_SRCS})

target_link_libraries(kr_common_utility_test
        kr_common_core
        kr_common_utility
        kr_common_log
        ${Boost_LOG_SETUP_LIBRARY}
        ${Boost_LOG_LIBRARY}
        ${Boost_ATOMIC_LIBRARY}
        ${Boost_DATE_TIME_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_REGEX_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
        ${FMT_LIBRARIES}
        )
add_test(NAME kr_common_utility_test COMMAND kr_common_utility_test)

set_target_properties(kr_common_utility_test PROPERTIES FOLDER CommonTargets/Utility)