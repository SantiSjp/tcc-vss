cmake_minimum_required(VERSION 3.8)
project(cicd_demo VERSION 0.0.1 LANGUAGES CXX)

set(PROJECT_NAME cicd_demo)

option(BUILD_TESTS "Build unit tests" OFF)

add_subdirectory(lib)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()
