set(TEST_NAME unit_test)

find_package(GTest REQUIRED)

file(GLOB_RECURSE TEST_SOURCES test*.cpp)

add_executable(${TEST_NAME}
    gtest_setup.cpp
    ${TEST_SOURCES}
)

target_link_libraries(${TEST_NAME} 
    PUBLIC 
        GTest::GTest 
        GTest::Main
        proprietary_libs
        pthread
        ${OpenCV_LIBS}
)

add_test(unit_tests ${TEST_NAME})
