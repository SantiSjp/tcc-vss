cmake_minimum_required(VERSION 3.13)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)

file(GLOB_RECURSE SOURCES include/*.cpp)
file(GLOB_RECURSE HEADERS include/*.hpp)

add_library(polym SHARED ${SOURCES} ${HEADERS})

set_target_properties(polym PROPERTIES PUBLIC_HEADER "include/Msg.hpp;include/Queue.hpp")

install (TARGETS polym
         ARCHIVE DESTINATION lib
         LIBRARY DESTINATION lib
		 PUBLIC_HEADER DESTINATION lib
         RUNTIME DESTINATION bin)

#add_executable(testpolym src/Test.cpp)
#target_link_libraries(testpolym pthread polym)
