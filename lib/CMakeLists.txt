#include src for vision exec compilation
add_subdirectory(src)

#include boost
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE( Boost 1.54.0 COMPONENTS filesystem system REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

#find files for lib compilation
file(GLOB_RECURSE SOURCE_FILES include/Vision/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/Vision/*.h)

#Create lib 
add_library(proprietary_libs STATIC ${SOURCE_FILES} ${HEADER_FILES})
set_target_properties(proprietary_libs PROPERTIES PUBLIC_HEADER "include/CommmandInterface.h;include/DataTypes.h")

#include polym manualy
file(GLOB_RECURSE POLYM_SOURCE include/PolyM/*.cpp)
file(GLOB_RECURSE POLYM_HEADER include/PolyM/*.h)
add_library(polym SHARED ${POLYM_SOURCE} ${POLYM_HEADER})

#directories for installation
target_include_directories(polym
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_features(proprietary_libs PUBLIC cxx_std_20)
target_include_directories(proprietary_libs
	PUBLIC
        $<INSTALL_INTERFACE:include>
     	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            ${CMAKE_CURRENT_SOURCE_DIR}/src)

#link libraries
target_link_libraries(proprietary_libs ${Boost_LIBRARIES} ${OpenCV_LIBS}  polym pthread stdc++fs inotify-cpp-shared)
