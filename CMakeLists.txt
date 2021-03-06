cmake_minimum_required(VERSION 3.12)

option(EMPTY_BUILD_EXAMPLE "Build the Empty example program" False)

# Generate enum struct headers from def files
find_package(Python3 REQUIRED)

file(GLOB_RECURSE ENUM_STRUCTS CONFIGURE_DEPENDS
	src/*.def
)

foreach(ENUM ${ENUM_STRUCTS})
	string(REPLACE ".def" ".cpp" ENUM_CPP ${ENUM})
	string(REPLACE ".def" ".hpp" ENUM_HPP ${ENUM})
	add_custom_command(OUTPUT ${ENUM_CPP} ${ENUM_HPP}
		DEPENDS ${ENUM}
		COMMAND ${Python3_EXECUTABLE} autogen.py ${CMAKE_CURRENT_SOURCE_DIR}/src ${ENUM} utils
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
	execute_process(COMMAND ${Python3_EXECUTABLE} autogen.py ${CMAKE_CURRENT_SOURCE_DIR}/src ${ENUM} utils
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Enum Definition Files" FILES ${ENUM_STRUCTS})

# Executable setup
project(Empty VERSION 0.1)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
	src/*.c src/*.cpp src/*.h src/*.hpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SOURCES})

add_library(Empty ${SOURCES} ${ENUM_STRUCTS})
target_include_directories(Empty PUBLIC src)

target_compile_features(Empty PRIVATE cxx_std_17)

### Build examples if requested

if(EMPTY_BUILD_EXAMPLE)
	add_subdirectory("example")
endif()

### Third-party libraries

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(ThirdParty/glfw)
target_link_libraries(Empty PUBLIC glfw)
