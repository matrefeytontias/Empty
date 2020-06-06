cmake_minimum_required(VERSION 3.12)

# Executable setup

add_executable(Empty_Sample main.cpp Mesh.h)

target_compile_features(Empty_Sample PRIVATE cxx_std_17)

add_custom_command(TARGET Empty_Sample POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/mctet.off ${CMAKE_CURRENT_BINARY_DIR}
)

### Third-party libraries

# Empty
target_link_libraries(Empty_Sample Empty)
