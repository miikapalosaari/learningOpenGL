include_directories("${GLFW_SOURCE_DIR}/deps")
set(GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/glad.h" "${GLFW_SOURCE_DIR}/deps/glad/glad.c")
include_directories("include")

add_executable(opengl_window main_opengl_window.cpp ${GLAD_GL})
target_link_libraries(opengl_window PUBLIC glfw)



