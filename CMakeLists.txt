cmake_minimum_required(VERSION 3.4)
project(vision_vss VERSION 0.0.1 LANGUAGES CXX)

set(PROJECT_NAME vision_vss)
set(VERSION 0.0.1)

option(BUILD_TESTS "Build unit tests" ON)
option(GEN_PACKAGE "Generate rpm for deployment" OFF)

add_subdirectory(lib)

find_package(OpenCV REQUIRED)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

if(GEN_PACKAGE)
    install(TARGETS vision_vss proprietary_libs
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static)

    set(CPACK_PACKAGE_NAME "vision_vss")
    set(CPACK_PACKAGE_VENDOR "jp&santi")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Core vision system for vsss.")
    set(CPACK_PACKAGE_VERSION ${VERSION})
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_PROCESSOR}")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "jp&santi")
    set(CPACK_SOURCE_IGNORE_FILES
        .git*
        .vscode
    )
    include(CPack)
endif()
