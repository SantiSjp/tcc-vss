cmake_minimum_required(VERSION 3.4)
project(vision_vss VERSION 0.3.0 LANGUAGES CXX)
set(PROJECT_NAME vision_vss)
set(VERSION 0.3.0)

set(CMAKE_CXX_STANDARD 20)

option(BUILD_TESTS "Build unit tests" ON)
option(GEN_PACKAGE "Generate packages for deployment" OFF)
option(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib)
add_subdirectory(3rdparty)

#Make necessary directories
file(MAKE_DIRECTORY /tmp/vision/capture)
file(MAKE_DIRECTORY /tmp/vision/test-output)

if(BUILD_TESTS)
    file(MAKE_DIRECTORY /tmp/vision/test/frames)
    file(MAKE_DIRECTORY /tmp/vision/test/field)
    file(MAKE_DIRECTORY /tmp/vision/test/color)
    
    file(COPY "test/images/clean_field/vss_field_clean.png" DESTINATION "/tmp/vision/test/field")
    
    file(GLOB TEST_COLORS "test/images/color/*.png")
    file(COPY ${TEST_COLORS} DESTINATION "/tmp/vision/test/color")
    
    file(GLOB TEST_FRAMES "test/images/frames/frame_*.png")
    file(COPY ${TEST_FRAMES} DESTINATION "/tmp/vision/test/frames")
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
