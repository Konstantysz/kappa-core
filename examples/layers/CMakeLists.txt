cmake_minimum_required(VERSION 3.26)
project(layers_example)

add_executable(layers_example main.cpp)

target_link_libraries(layers_example PRIVATE Kappa)

target_compile_features(layers_example PRIVATE cxx_std_20)
