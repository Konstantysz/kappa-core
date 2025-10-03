cmake_minimum_required(VERSION 3.26)
project(events_example)

add_executable(events_example main.cpp)

target_link_libraries(events_example PRIVATE Core)

target_compile_features(events_example PRIVATE cxx_std_20)
