cmake_minimum_required(VERSION 3.26)
project(kappa-core
    VERSION 0.5.1
    DESCRIPTION "Lightweight C++20 application framework with layer-based architecture"
    LANGUAGES CXX)

# ========================================
# Build Options
# ========================================
# Only build examples and tests by default when this is the main project
# When used as a submodule, these should be OFF by default to avoid cluttering parent builds
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    option(BUILD_EXAMPLES "Build example applications" ON)
    option(BUILD_TESTS "Build test executables" ON)
else()
    option(BUILD_EXAMPLES "Build example applications" OFF)
    option(BUILD_TESTS "Build test executables" OFF)
endif()
option(ENABLE_COVERAGE "Enable code coverage analysis" OFF)

# ========================================
# LLVM Coverage Integration
# ========================================
if(ENABLE_COVERAGE)
    find_program(LLVM_PROFDATA_EXE
        NAMES "llvm-profdata"
        PATHS "C:/Program Files/LLVM/bin"
    )
    find_program(LLVM_COV_EXE
        NAMES "llvm-cov"
        PATHS "C:/Program Files/LLVM/bin"
    )

    if(LLVM_PROFDATA_EXE AND LLVM_COV_EXE)
        message(STATUS "LLVM coverage tools found:")
        message(STATUS "  llvm-profdata: ${LLVM_PROFDATA_EXE}")
        message(STATUS "  llvm-cov: ${LLVM_COV_EXE}")

        set(LLVM_PROFDATA_EXE ${LLVM_PROFDATA_EXE} CACHE INTERNAL "")
        set(LLVM_COV_EXE ${LLVM_COV_EXE} CACHE INTERNAL "")

        add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
        add_link_options(-fprofile-instr-generate -fcoverage-mapping)

        message(STATUS "Code coverage enabled for kappa-core")
    else()
        message(FATAL_ERROR "LLVM coverage tools not found! Please ensure llvm-profdata and llvm-cov are in your PATH.")
    endif()
endif()

# Include code quality functions
include(cmake/CodeQuality.cmake)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_library(Kappa STATIC
    src/Application.cpp
    src/Window.cpp
    src/WindowStatePersistence.cpp
    src/Texture.cpp)

target_include_directories(Kappa PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(Kappa PUBLIC
    glad::glad
    glfw
    glm::glm
    nlohmann_json::nlohmann_json
    spdlog::spdlog)

target_compile_features(Kappa PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(Kappa PRIVATE /W4)
else()
    target_compile_options(Kappa PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# Setup code quality targets and IDE integration
kappa_add_code_quality_targets()
kappa_setup_ide_integration()

# Ensure consistent iterator debug level in Debug builds (MSVC)
if(MSVC)
    target_compile_definitions(Kappa PUBLIC
        $<$<CONFIG:Debug>:_ITERATOR_DEBUG_LEVEL=2>
        $<$<CONFIG:Release>:_ITERATOR_DEBUG_LEVEL=0>
    )
endif()

# Build examples
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Build tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
