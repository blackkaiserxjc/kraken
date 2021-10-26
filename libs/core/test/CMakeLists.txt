find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

set(kr_common_core_test_SRCS
    expected_unittest.cpp
    unittest_main.cpp
)

add_executable(kr_common_core_test ${kr_common_core_test_SRCS})

target_link_libraries(kr_common_core_test
    kr_common_core
)
set_target_properties(kr_common_core_test PROPERTIES FOLDER CommonTargets/Core)
