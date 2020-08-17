add_subdirectory(src)

file(GLOB_RECURSE SOURCE_FILES include/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.h)

file(GLOB_RECURSE POLYM_SOURCE include/PolyM/*.cpp)
file(GLOB_RECURSE POLYM_HEADER include/PolyM/*.h)

add_library(proprietary_libs SHARED ${SOURCE_FILES} ${HEADER_FILES})
add_library(polym SHARED ${POLYM_SOURCE} ${POLYM_HEADER})

# target_compile_features(proprietary_libs PUBLIC cxx_std_17)
# target_include_directories(proprietary_libs
# 	PUBLIC
#        	$<INSTALL_INTERFACE:include>
#            	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#                ${CMAKE_CURRENT_SOURCE_DIR}/src)
