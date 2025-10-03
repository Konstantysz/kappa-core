cmake_minimum_required(VERSION 3.26)
project(kappa_core_minimal_example LANGUAGES CXX)

# Only add kappa-core if building standalone
if(NOT TARGET Kappa)
    add_subdirectory(../.. kappa-core-lib)
endif()

# Create example executable
add_executable(MinimalExample main.cpp)

# Link against Kappa
target_link_libraries(MinimalExample PRIVATE Kappa)

# Set C++20
target_compile_features(MinimalExample PRIVATE cxx_std_20)
