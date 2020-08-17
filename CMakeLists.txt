cmake_minimum_required(VERSION 3.8)
project(vision_vss VERSION 0.0.1 LANGUAGES CXX)

set(PROJECT_NAME vision_vss)

option(BUILD_TESTS "Build unit tests" OFF)

add_subdirectory(lib)

find_package(OpenCV REQUIRED)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()
