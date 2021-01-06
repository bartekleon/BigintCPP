FIND_PROGRAM( GCOV_PATH gcov )
FIND_PROGRAM( LCOV_PATH NAMES lcov lcov.bat lcov.exe lcov.perl )
FIND_PROGRAM( GENHTML_PATH NAMES genhtml genhtml.perl genhtml.bat )
FIND_PROGRAM( GCOVR_PATH gcovr PATHS ${CMAKE_SOURCE_DIR}/test)
find_program( GENINFO_BIN geninfo )

find_package_handle_standard_args(lcov REQUIRED_VARS LCOV_PATH GENINFO_BIN GENHTML_PATH)

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

SET(COVERAGE_COMPILER_FLAGS "-g -fprofile-arcs -ftest-coverage"
    CACHE INTERNAL "")

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

IF(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    MESSAGE(WARNING "Code coverage results with an optimised (non-Debug) build may be misleading")
ENDIF() # NOT CMAKE_BUILD_TYPE STREQUAL "Debug"

IF(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_Fortran_COMPILER_ID STREQUAL "GNU")
    link_libraries(gcov)
ENDIF()


FUNCTION(SETUP_TARGET_FOR_COVERAGE _targetname _testrunner)
    IF(NOT LCOV_PATH)
        MESSAGE(FATAL_ERROR "lcov not found! Aborting...")
    ENDIF() # NOT LCOV_PATH

    IF(NOT GENHTML_PATH)
        MESSAGE(FATAL_ERROR "genhtml not found! Aborting...")
    ENDIF() # NOT GENHTML_PATH

    SEPARATE_ARGUMENTS(test_command UNIX_COMMAND "${_testrunner}")

    SET(BASEDIR ${PROJECT_SOURCE_DIR})

    # Setup target
    ADD_CUSTOM_TARGET(${_targetname}
        # Cleanup lcov
    COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} -directory . -b ${BASEDIR} --zerocounters
    # Create baseline to make sure untouched files show up in the report
    COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} -c -i -d . -b ${BASEDIR} -o ${_targetname}.base

    # Run tests
    COMMAND ${test_command} ${ARGV3}

    COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} --directory . -b ${BASEDIR} --capture --output-file ${_targetname}.capture
    COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} -a ${_targetname}.base -a ${_targetname}.capture --output-file ${_targetname}.total
    COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} --output-file ${_targetname}.info

    # Generate HTML output
    COMMAND ${GENHTML_PATH} -o ${_targetname} ${_targetname}.info

    # Set output files as GENERATED (will be removed on 'make clean')
    BYPRODUCTS
        ${_targetname}.base
        ${_targetname}.capture
        ${_targetname}.total
        ${_targetname}.info
        ${_targetname}  # report directory

    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Resetting code coverage counters to zero.\nProcessing code coverage counters and generating report."
    )

ENDFUNCTION() # SETUP_TARGET_FOR_COVERAGE
