# Specify minimum supported cmake version
cmake_minimum_required(VERSION 3.5)

#PROJECT NAME
project(LearningOpenGL)

include_directories("${GLFW_SOURCE_DIR}/deps")
add_subdirectory("ext")
add_subdirectory("src")



