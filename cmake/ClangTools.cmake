find_program(CLANG_TIDY_COMMAND NAMES "clang-tidy" "clang-tidy-10"
  DOC "Path to clang-tidy executable")
if(NOT CLANG_TIDY_COMMAND)
    message(FATAL_ERROR "CMake_RUN_CLANG_TOOLS is ON but clang-tidy is not found!")
else()
  message(STATUS "clang-tidy found: ${CLANG_TIDY_COMMAND}")
  add_custom_target(
    clang-tidy
    COMMAND ${CLANG_TIDY_COMMAND}
    ${SOURCE_FILES}
    --
    -std=c++17
    -I ${CMAKE_SOURCE_DIR}/include
  )
endif()

find_program(CLANG_FORMAT NAMES "clang-format" "clang-format-10")
if(NOT CLANG_FORMAT)
  message(FATAL_ERROR "CMake_RUN_CLANG_TOOLS is ON but clang-format is not found!")
else()
  message(STATUS "clang-format found: ${CLANG_FORMAT}")
  add_custom_target(
    clang-format
    COMMAND ${CLANG_FORMAT}
    -i
    ${SOURCE_FILES} ${INCLUDE_FILES} ${TEST_FILES}
  )
endif ()