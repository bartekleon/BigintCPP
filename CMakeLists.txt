cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)
set(PROJECT_NAME Bigint)
project(${PROJECT_NAME})

include_directories(include)
add_subdirectory(src)

file(GLOB TEST_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)

target_link_libraries(${PROJECT_NAME}_lib)
add_subdirectory(googletest-release-1.8.0)
add_subdirectory(test)
