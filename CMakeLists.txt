# CMakeList.txt : CMake project for yac8emul, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project ("yac8emul" CXX)

OPTION(BUILD_3DS "Build the 3DS version of this emulator" off)

# Add source to this project's
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_subdirectory(src)
add_subdirectory(vendor)

if (BUILD_3DS)
	add_subdirectory(3ds)
endif ()

if (BUILD_TESTING)
	add_subdirectory(test)
endif ()
