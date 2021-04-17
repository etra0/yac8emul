# CMakeList.txt : CMake project for yac8emul, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(Tools3DS)

find_package(CTRULIB REQUIRED)

set(BUILD_TESTING OFF)
set(FORCE_SMDHTOOL ON)

add_executable(
	yac8emul-3ds
	main.cpp
)

target_link_libraries(
	yac8emul-3ds
	yac8emul-lib
	3ds::ctrulib
)

add_cia_target(yac8emul-3ds data/spec.rsf data/icon.png data/sound.wav)
