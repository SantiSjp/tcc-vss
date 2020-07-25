include_directories(../include)

add_executable (${PROJECT_NAME} test.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC proprietary_libs)
