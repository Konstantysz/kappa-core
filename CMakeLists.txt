cmake_minimum_required(VERSION 3.26)
project(kappa-core
    VERSION 0.3.1
    DESCRIPTION "Lightweight C++20 application framework with layer-based architecture"
    LANGUAGES CXX)

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

# Build examples (optional, can be disabled with -DBUILD_EXAMPLES=OFF)
option(BUILD_EXAMPLES "Build example applications" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
