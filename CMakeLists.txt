cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)
set(PROJECT_NAME Bigint)
project(${PROJECT_NAME})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include_directories(include)

file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(Bigint_lib ${SOURCE_FILES})

file(GLOB TEST_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)

target_link_libraries(${PROJECT_NAME}_lib)
include(Test)
