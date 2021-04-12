# CMakeList.txt : CMake project for yac8emul, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project ("yac8emul")

# Add source to this project's
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_subdirectory(src)
add_subdirectory(vendor)

include(CTest)
if (BUILD_TESTING)
	add_subdirectory(test)
endif ()

# TODO: Add tests and install targets if needed.
