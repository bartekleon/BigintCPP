enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(unit_tests ${TEST_FILES})

target_link_libraries(unit_tests gtest gtest_main Bigint_lib pthread)

add_test(UnitTests unit_tests)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")

include(CodeCoverage)
setup_target_for_coverage(Bigint_coverage unit_tests coverage)
