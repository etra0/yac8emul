cmake_minimum_required (VERSION 3.8)

add_executable(yac8emul main.cpp "yac8emul/errors.h")

# TODO: Remove this
add_definitions(-DYAC8EMUL_VERBOSE)

add_library(
	yac8emul-lib
	interpreter.cpp
  yac8emul/errors.h
  yac8emul/graphics.h
  )

target_include_directories(yac8emul-lib PUBLIC .)

target_link_libraries(
  yac8emul
  PRIVATE
  yac8emul-lib
  SDL2::SDL2
  ${SDL2_LIBRARIES}
)

if (MSVC)
	target_compile_options(yac8emul PUBLIC /W4)
endif()
