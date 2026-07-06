cmake_minimum_required(VERSION 3.20)

project(engelbart LANGUAGES CXX)

add_library(engelbart INTERFACE)
target_include_directories(engelbart INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_compile_features(engelbart INTERFACE cxx_std_20)

option(ENGELBART_BUILD_TESTS "Build Engelbart tests" ON)
option(ENGELBART_BUILD_EXAMPLES "Build Engelbart examples" ON)

if(ENGELBART_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(ENGELBART_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
