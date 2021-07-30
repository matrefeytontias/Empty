cmake_minimum_required(VERSION 3.12)

project(EMPTY VERSION 0.1 DESCRIPTION "Type-safe wrapper and utility for OpenGL 4.6")

option(EMPTY_BUILD_EXAMPLE "Build the Empty example program" False)

add_subdirectory(Empty)
if(EMPTY_BUILD_EXAMPLE)
	add_subdirectory(example)
	set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT Empty_Sample)
endif()
