find_package(Boost REQUIRED atomic date_time filesystem log log_setup regex system thread unit_test_framework)

include_directories(${Boost_INCLUDE_DIRS})

set(kr_common_log_test_SRCS
    log_unittest.cpp
    unittest_main.cpp
)

add_executable(kr_common_log_test ${kr_common_log_test_SRCS})
target_link_libraries(kr_common_log_test
    kr_common_log
    ${Boost_ATOMIC_LIBRARY}
    ${Boost_DATE_TIME_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_LOG_LIBRARY}
    ${Boost_LOG_SETUP_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
)

add_test(NAME kr_common_log_test COMMAND kr_common_log_test)

set_target_properties(kr_common_log_test PROPERTIES FOLDER CommonTargets/Utility)