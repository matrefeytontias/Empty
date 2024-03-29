cmake_minimum_required(VERSION 3.12)

# Executable setup

add_executable(Empty_Sample main.cpp Mesh.h Camera.h stb_image.h)

target_compile_features(Empty_Sample PRIVATE cxx_std_17)

set(RESOURCES cube.obj mctet.off Compute.glsl DeferredVertex.glsl DeferredFragment.glsl Vertex.glsl Fragment.glsl texture.png)
target_sources(Empty_Sample PRIVATE ${RESOURCES})

foreach(RES ${RESOURCES})
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${RES}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${RES} ${CMAKE_CURRENT_BINARY_DIR}/${RES}
        MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/${RES})
endforeach()

### Third-party libraries

# Empty

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(ThirdParty/glfw)

target_link_libraries(Empty_Sample PUBLIC Empty glfw)
