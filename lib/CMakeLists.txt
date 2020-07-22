add_subdirectory(src)

file(GLOB_RECURSE SOURCE_FILES include/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.h)

add_library(proprietary_libs SHARED ${SOURCE_FILES} ${HEADER_FILES})

# target_compile_features(proprietary_libs PUBLIC cxx_std_17)
# target_include_directories(proprietary_libs
# 	PUBLIC
#        	$<INSTALL_INTERFACE:include>
#            	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#                ${CMAKE_CURRENT_SOURCE_DIR}/src)
