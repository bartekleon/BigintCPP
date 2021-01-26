cmake_minimum_required(VERSION 3.17)

set(PROJECT_NAME Bigint)
project(${PROJECT_NAME})

# Require C++17.
# Change this value to require other C++ standards (14, 17, etc.)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

option(BUILD_TESTS "Build tests, run them and check coverage" OFF)
option(RUN_CLANG_TOOLS "Enable Clang tooling" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include_directories(include)

# Use globbing to find source files.
# Any source files added to include/ or src/ will automatically be added to the project.
file(GLOB_RECURSE INCLUDE_FILES ${CMAKE_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE TEST_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)

if(NOT BUILD_TESTS)
  if(MSVC)
    add_compile_options(/permissive- /W4 /w14640 /WX)
  else()
    add_compile_options(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Werror)
  endif()
endif()

add_library(${PROJECT_NAME} ${SOURCE_FILES})
# If external libraries are needed, use find_library() and target_link_libraries()
# to dynamically link them to your executable. Generally, you'll want to install
# them onto the host using your OS's package manager.
# Example:
#   find_library(LIB_FMT fmt)
#   target_link_libraries(${PROJECT_NAME} ${LIB_FMT})

if(BUILD_TESTS)
  include(Test)
endif()

if(RUN_CLANG_TOOLS)
  include(ClangTools)
endif()