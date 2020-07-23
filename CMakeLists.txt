cmake_minimum_required(VERSION 3.12)

project(stockstufflib)

option(BUILD_TESTS "Build the tests" ON)

add_subdirectory(src)

if(BUILD_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()