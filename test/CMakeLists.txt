cmake_minimum_required(VERSION 3.8)

add_executable(
	yac8emul-test
	test.cpp
)

target_link_libraries(
	yac8emul-test
	PRIVATE
	doctest
	doctest_with_main
	yac8emul-lib
)