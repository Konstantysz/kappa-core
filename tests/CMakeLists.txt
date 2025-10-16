cmake_minimum_required(VERSION 3.26)

# Gradually re-enable tests to find the culprit
add_executable(TestKappaCore
    TestSimple.cpp
    TestEventBus.cpp  # ✅ Passed (15 tests)
    TestLayer.cpp     # ✅ Passed (15 tests)
    TestWindow.cpp    # Testing Window structures
    TestWindowStatePersistence.cpp  # Testing JSON persistence
    TestApplication.cpp  # Most complex - Application with layers
)

target_compile_features(TestKappaCore PRIVATE cxx_std_20)

target_include_directories(TestKappaCore PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

find_package(GTest CONFIG REQUIRED)

target_link_libraries(TestKappaCore
    PRIVATE
    GTest::gtest
    GTest::gtest_main
    Kappa
)

# Ensure same runtime library settings as Kappa
set_target_properties(TestKappaCore PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
)

# Match iterator debug level with Kappa library (MSVC)
if(MSVC)
    target_compile_definitions(TestKappaCore PRIVATE
        $<$<CONFIG:Debug>:_ITERATOR_DEBUG_LEVEL=2>
        $<$<CONFIG:Release>:_ITERATOR_DEBUG_LEVEL=0>
    )
endif()

add_test(NAME TestKappaCore COMMAND TestKappaCore)
