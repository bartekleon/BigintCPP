FIND_PROGRAM( GCOV_PATH gcov )
FIND_PROGRAM( LCOV_PATH lcov )

IF(NOT LCOV_PATH)
    MESSAGE(FATAL_ERROR "lcov not found! Aborting...")
ENDIF() # NOT LCOV_PATH

IF(NOT GCOV_PATH)
    MESSAGE(FATAL_ERROR "gcov not found! Aborting...")
ENDIF() # NOT GCOV_PATH

IF("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
    IF("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 3)
        MESSAGE(FATAL_ERROR "Clang version must be 3.0.0 or greater! Aborting...")
    ENDIF()
ELSEIF(NOT CMAKE_COMPILER_IS_GNUCXX)
    MESSAGE(FATAL_ERROR "Compiler is not GNU gcc! Aborting...")
ENDIF() # CHECK VALID COMPILER

SET(CMAKE_CXX_FLAGS_COVERAGE
        "-g -O0 --coverage -fprofile-arcs -ftest-coverage"
        CACHE STRING "Flags used by the C++ compiler during coverage builds."
        FORCE )
SET(CMAKE_C_FLAGS_COVERAGE
        "-g -O0 --coverage -fprofile-arcs -ftest-coverage"
        CACHE STRING "Flags used by the C compiler during coverage builds."
        FORCE )
SET(CMAKE_EXE_LINKER_FLAGS_COVERAGE
        ""
        CACHE STRING "Flags used for linking binaries during coverage builds."
        FORCE )
SET(CMAKE_SHARED_LINKER_FLAGS_COVERAGE
        ""
        CACHE STRING "Flags used by the shared libraries linker during coverage builds."
        FORCE )
MARK_AS_ADVANCED(
        CMAKE_CXX_FLAGS_COVERAGE
        CMAKE_C_FLAGS_COVERAGE
        CMAKE_EXE_LINKER_FLAGS_COVERAGE
        CMAKE_SHARED_LINKER_FLAGS_COVERAGE )

IF ( NOT (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Coverage"))
    MESSAGE( WARNING "Code coverage results with an optimized (non-Debug) build may be misleading" )
ENDIF() # NOT CMAKE_BUILD_TYPE STREQUAL "Debug"

FUNCTION(SETUP_TARGET_FOR_COVERAGE _targetname _testrunner)
    SEPARATE_ARGUMENTS(test_command UNIX_COMMAND "${_testrunner}")

    # Setup target
    ADD_CUSTOM_TARGET(${_targetname}

        # Cleanup lcov
        ${LCOV_PATH} --directory . --zerocounters

        # Run tests
        COMMAND ${test_command} ${ARGV3}

        # Capturing lcov counters and generating report
        COMMAND ${LCOV_PATH} --directory . --base-directory . --capture --output-file coverage.info
        COMMAND ${LCOV_PATH} --remove coverage.info '/usr*' '*/test/*' -o coverage.info

        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Resetting code coverage counters to zero.\nProcessing code coverage counters and generating report."
    )

ENDFUNCTION() # SETUP_TARGET_FOR_COVERAGE