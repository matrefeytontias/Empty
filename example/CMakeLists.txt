cmake_minimum_required(VERSION 3.12)

# Executable setup

add_executable(Empty_Sample main.cpp Mesh.h)

target_compile_features(Empty_Sample PRIVATE cxx_std_17)

target_sources(Empty_Sample PRIVATE Fragment.glsl Vertex.glsl)

add_custom_command(TARGET Empty_Sample POST_BUILD
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy mctet.off ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy Fragment.glsl ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy Vertex.glsl ${CMAKE_CURRENT_BINARY_DIR}
)

### Third-party libraries

# Empty
target_link_libraries(Empty_Sample Empty)
