# CMakeList.txt : CMake project for yac8emul, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(Tools3DS)
add_3dsx_target(yac8emul NO_SMDH)

add_executable(
	yac8emul-3ds
	main.cpp
)

target_link_libraries(
	yac8emul-3ds
	yac8emul-lib
)

