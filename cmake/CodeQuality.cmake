# ========================================
# Code Quality CMake Functions
# ========================================

# Function to add clang-tidy target for a specific directory
function(add_clang_tidy_target TARGET_NAME SOURCE_DIR)
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy" PATHS "C:/Program Files/LLVM/bin")

    if(CLANG_TIDY_EXE)
        # Get all C++ source files
        file(GLOB_RECURSE SOURCE_FILES
            "${SOURCE_DIR}/*.cpp"
            "${SOURCE_DIR}/*.hpp"
            "${SOURCE_DIR}/*.c"
            "${SOURCE_DIR}/*.h"
            "${SOURCE_DIR}/*.cc"
            "${SOURCE_DIR}/*.cxx"
        )

        # Create clang-tidy target
        add_custom_target(${TARGET_NAME}
            COMMAND ${CLANG_TIDY_EXE}
                --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy
                --header-filter=${CMAKE_SOURCE_DIR}/src/.*
                --format-style=file
                -p ${CMAKE_BINARY_DIR}
                ${SOURCE_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Running clang-tidy on ${SOURCE_DIR}"
            VERBATIM
        )

        message(STATUS "Added clang-tidy target: ${TARGET_NAME}")
    else()
        message(WARNING "clang-tidy not found, cannot create target: ${TARGET_NAME}")
    endif()
endfunction()

# Function to add clang-format target
function(add_clang_format_target TARGET_NAME SOURCE_DIR)
    find_program(CLANG_FORMAT_EXE NAMES "clang-format" PATHS "C:/Program Files/LLVM/bin")

    if(CLANG_FORMAT_EXE)
        # Get all C++ source files
        file(GLOB_RECURSE SOURCE_FILES
            "${SOURCE_DIR}/*.cpp"
            "${SOURCE_DIR}/*.hpp"
            "${SOURCE_DIR}/*.c"
            "${SOURCE_DIR}/*.h"
            "${SOURCE_DIR}/*.cc"
            "${SOURCE_DIR}/*.cxx"
        )

        # Create format target
        add_custom_target(${TARGET_NAME}
            COMMAND ${CLANG_FORMAT_EXE}
                -i
                -style=file
                ${SOURCE_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Formatting code with clang-format in ${SOURCE_DIR}"
            VERBATIM
        )

        # Create format-check target (doesn't modify files)
        add_custom_target(${TARGET_NAME}-check
            COMMAND ${CLANG_FORMAT_EXE}
                --dry-run
                --Werror
                -style=file
                ${SOURCE_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Checking code format with clang-format in ${SOURCE_DIR}"
            VERBATIM
        )

        message(STATUS "Added clang-format targets: ${TARGET_NAME}, ${TARGET_NAME}-check")
    else()
        message(WARNING "clang-format not found, cannot create target: ${TARGET_NAME}")
    endif()
endfunction()

# Function to add comprehensive code quality target for kappa-core
function(kappa_add_code_quality_targets)
    # Add coverage targets if coverage is enabled
    if(ENABLE_COVERAGE)
        kappa_add_coverage_targets()
    endif()

    message(STATUS "Kappa-Core code quality targets configured")
    if(ENABLE_COVERAGE)
        message(STATUS "  - Coverage: kappa-coverage, kappa-coverage-html, kappa-coverage-report, kappa-coverage-clean")
    endif()
endfunction()

# Function to setup IDE integration for kappa-core
function(kappa_setup_ide_integration)
    # For CLion/Visual Studio - add compile_commands.json
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON PARENT_SCOPE)
    message(STATUS "IDE integration configured (compile_commands.json enabled)")
endfunction()

# Function to add static analysis options to a target
function(add_static_analysis_to_target TARGET_NAME)
    if(ENABLE_CLANG_TIDY AND CLANG_TIDY_EXE)
        set_target_properties(${TARGET_NAME} PROPERTIES
            CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
        )
        message(STATUS "Added clang-tidy to target: ${TARGET_NAME}")
    endif()
endfunction()

# ========================================
# Coverage Functions
# ========================================

# Function to add coverage targets for kappa-core
function(kappa_add_coverage_targets)
    if(NOT ENABLE_COVERAGE)
        return()
    endif()

    # Verify LLVM tools are available
    if(NOT LLVM_PROFDATA_EXE OR NOT LLVM_COV_EXE)
        message(WARNING "Coverage enabled but LLVM tools not found. Coverage targets will not be created.")
        return()
    endif()

    # Set coverage output directories
    set(COVERAGE_DIR "${CMAKE_BINARY_DIR}/coverage")
    set(COVERAGE_RAW_DIR "${COVERAGE_DIR}/raw")
    set(COVERAGE_HTML_DIR "${COVERAGE_DIR}/html")
    set(COVERAGE_DATA_FILE "${COVERAGE_DIR}/coverage.profdata")

    # Get test executable for kappa-core
    set(TEST_EXECUTABLE "${CMAKE_BINARY_DIR}/tests/TestKappaCore${CMAKE_EXECUTABLE_SUFFIX}")

    # Get all source files for coverage (only src and include, not tests or external)
    file(GLOB_RECURSE COVERAGE_SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_SOURCE_DIR}/include/*.h"
    )

    # Target: kappa-coverage-clean - Clean coverage data
    add_custom_target(kappa-coverage-clean
        COMMAND ${CMAKE_COMMAND} -E rm -rf "${COVERAGE_DIR}"
        COMMAND ${CMAKE_COMMAND} -E rm -f "${CMAKE_BINARY_DIR}/tests/default.profraw"
        COMMAND ${CMAKE_COMMAND} -E rm -f "default.profraw"
        COMMENT "Cleaning coverage data"
        VERBATIM
    )

    # Target: kappa-coverage-run - Run tests to generate coverage data
    add_custom_target(kappa-coverage-run
        COMMAND ${CMAKE_COMMAND} -E make_directory "${COVERAGE_RAW_DIR}"
        COMMAND ${CMAKE_COMMAND} -E env LLVM_PROFILE_FILE=${COVERAGE_RAW_DIR}/coverage-%p.profraw
                "${TEST_EXECUTABLE}"
        DEPENDS TestKappaCore
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running tests to generate coverage data"
        VERBATIM
    )

    # Target: kappa-coverage-process - Process raw coverage data
    add_custom_target(kappa-coverage-process
        COMMAND ${CMAKE_COMMAND} -E make_directory "${COVERAGE_DIR}"
        COMMAND ${LLVM_PROFDATA_EXE} merge
                -sparse
                "${COVERAGE_RAW_DIR}/coverage-*.profraw"
                -o "${COVERAGE_DATA_FILE}"
        DEPENDS kappa-coverage-run
        COMMENT "Processing coverage data with llvm-profdata"
        VERBATIM
    )

    # Target: kappa-coverage-report - Generate text coverage report
    add_custom_target(kappa-coverage-report
        COMMAND ${LLVM_COV_EXE} report
                "${TEST_EXECUTABLE}"
                -instr-profile="${COVERAGE_DATA_FILE}"
                ${COVERAGE_SOURCE_FILES}
        DEPENDS kappa-coverage-process
        COMMENT "Generating coverage report"
        VERBATIM
    )

    # Target: kappa-coverage-html - Generate HTML coverage report
    add_custom_target(kappa-coverage-html
        COMMAND ${CMAKE_COMMAND} -E make_directory "${COVERAGE_HTML_DIR}"
        COMMAND ${LLVM_COV_EXE} show
                "${TEST_EXECUTABLE}"
                -instr-profile=${COVERAGE_DATA_FILE}
                -format=html
                -output-dir=${COVERAGE_HTML_DIR}
                -show-line-counts-or-regions
                -show-instantiation-summary
                ${COVERAGE_SOURCE_FILES}
        COMMAND ${CMAKE_COMMAND} -E echo "Coverage HTML report generated: ${COVERAGE_HTML_DIR}/index.html"
        DEPENDS kappa-coverage-process
        COMMENT "Generating HTML coverage report"
        VERBATIM
    )

    # Target: kappa-coverage-summary - Generate summary with statistics
    add_custom_target(kappa-coverage-summary
        COMMAND ${LLVM_COV_EXE} report
                "${TEST_EXECUTABLE}"
                -instr-profile="${COVERAGE_DATA_FILE}"
                -show-region-summary=false
                ${COVERAGE_SOURCE_FILES}
        DEPENDS kappa-coverage-process
        COMMENT "Generating coverage summary"
        VERBATIM
    )

    # Target: kappa-coverage - Main coverage target (generates both reports)
    add_custom_target(kappa-coverage
        DEPENDS kappa-coverage-html kappa-coverage-report
        COMMENT "Generating all coverage reports"
    )

    # Target: kappa-coverage-open - Open HTML report in browser (Windows)
    if(WIN32)
        add_custom_target(kappa-coverage-open
            COMMAND cmd /c start "" "${COVERAGE_HTML_DIR}/index.html"
            DEPENDS kappa-coverage-html
            COMMENT "Opening coverage report in browser"
            VERBATIM
        )
    endif()

    message(STATUS "Added coverage targets: kappa-coverage, kappa-coverage-html, kappa-coverage-report, kappa-coverage-clean, kappa-coverage-open")
endfunction()
