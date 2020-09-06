include_directories(../include)

add_executable (${PROJECT_NAME} vision.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC proprietary_libs)
