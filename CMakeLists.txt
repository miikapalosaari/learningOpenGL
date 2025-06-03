# Specify minimum supported cmake version
cmake_minimum_required(VERSION 3.5)

set(GLFW_SOURCE_DIR "${CMAKE_SOURCE_DIR}/ext/glfw-3.4")
set(STB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/ext/stb-master")

#PROJECT NAME
project(LearningOpenGL)

include_directories("${GLFW_SOURCE_DIR}/deps")
add_subdirectory("ext")
add_subdirectory("src")

# Define opengl_common INTERFACE target
add_library(opengl_common INTERFACE)

target_include_directories(opengl_common INTERFACE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/common
    ${GLFW_SOURCE_DIR}/deps
    ${STB_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/ext/glm-master
)

target_link_libraries(opengl_common INTERFACE glfw)

# Platform-specific system libs (Linux)
if(UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(opengl_common INTERFACE
        OpenGL::GL
        X11
        Xrandr
        Xi
        Xxf86vm
        Xcursor
        pthread
        dl
        m
    )
endif()




