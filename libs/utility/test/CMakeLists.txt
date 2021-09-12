find_package(Boost REQUIRED COMPONENTS atomic date_time regex system thread filesystem log_setup log unit_test_framework)
find_package(Fmt REQUIRED)
find_package(Threads REQUIRED)

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
        Boost::log_setup
        Boost::log
        Boost::atomic
        Boost::date_time
        Boost::filesystem
        Boost::regex
        Boost::system
        Boost::thread
        Boost::unit_test_framework
        fmt::fmt
        Threads::Threads
        )
#add_test(NAME kr_common_utility_test COMMAND kr_common_utility_test)

set_target_properties(kr_common_utility_test PROPERTIES FOLDER CommonTargets/Utility)