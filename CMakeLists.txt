# CMake entry point
cmake_minimum_required (VERSION 3.5)
project (images_primitives)

# Compile lib dependencies 
add_subdirectory (lib)

## Add headers files
include_directories (
	includes/
)


set(SOURCES src/main.cpp)

add_compile_options(
	-std=c++11
	-g
) 

# renderer
add_definitions(-D_FORWARD=1)


# main.cpp
add_executable(main ${SOURCES})
target_link_libraries(main EASY_BMP)
