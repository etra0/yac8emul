cmake_minimum_required (VERSION 3.8)

add_library(
	yac8emul-lib
	main.cpp
	interpreter.cpp
)

target_include_directories(yac8emul-lib PUBLIC .)