include_directories(../include)

add_executable (example_program test.cpp)
target_link_libraries(example_program PUBLIC proprietary_libs)
