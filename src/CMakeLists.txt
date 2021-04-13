cmake_minimum_required (VERSION 3.8)

add_executable(yac8emul main.cpp)

add_library(
	yac8emul-lib
	interpreter.cpp
)

target_include_directories(yac8emul-lib PUBLIC .)
target_link_libraries(yac8emul PRIVATE yac8emul-lib)