include_directories("${GLFW_SOURCE_DIR}/deps")

file(GLOB_RECURSE COMMON_INC_FILES "${CMAKE_SOURCE_DIR}/include/common/*.h")
file(GLOB_RECURSE COMMON_SRC_FILES "${CMAKE_SOURCE_DIR}/src/common/*.cpp")

set(GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/glad.c")

add_executable(opengl_window main_opengl_window.cpp ${GLAD_GL})
target_link_libraries(opengl_window PUBLIC glfw)

add_executable(shader_class_v1 main_shader_class_v1.cpp ${GLAD_GL} ${COMMON_INC_FILES} ${COMMON_SRC_FILES})
target_link_libraries(shader_class_v1 PUBLIC glfw)

add_executable(shader_class_v2 main_shader_class_v2.cpp ${GLAD_GL} ${COMMON_INC_FILES} ${COMMON_SRC_FILES})
target_link_libraries(shader_class_v2 PUBLIC glfw)

add_executable(spritebatch_test main_spritebatch_test.cpp ${GLAD_GL} ${COMMON_INC_FILES} ${COMMON_SRC_FILES})
target_link_libraries(spritebatch_test PUBLIC glfw)

add_executable(lighting_test_v1 main_lighting_test_v1.cpp ${GLAD_GL} ${COMMON_INC_FILES} ${COMMON_SRC_FILES})
target_link_libraries(lighting_test_v1 PUBLIC glfw)

add_executable(lighting_test_v2 main_lighting_test_v2.cpp ${GLAD_GL} ${COMMON_INC_FILES} ${COMMON_SRC_FILES})
target_link_libraries(lighting_test_v2 PUBLIC glfw)

add_executable(snake_game snake_game.cpp ${GLAD_GL} ${COMMON_INC_FILES} ${COMMON_SRC_FILES})
target_link_libraries(snake_game PUBLIC glfw)

# Ensure correct include paths
target_include_directories(opengl_window PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/common
    ${GLFW_SOURCE_DIR}/deps
)


# Ensure correct include paths
target_include_directories(shader_class_v1 PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/common
    ${GLFW_SOURCE_DIR}/deps
    ${STB_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/ext/glm-master
)

# Ensure correct include paths
target_include_directories(shader_class_v2 PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/common
    ${GLFW_SOURCE_DIR}/deps
    ${STB_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/ext/glm-master
)

# Ensure correct include paths
target_include_directories(spritebatch_test PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/common
    ${GLFW_SOURCE_DIR}/deps
    ${STB_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/ext/glm-master
)

# Ensure correct include paths
target_include_directories(lighting_test_v1 PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/common
    ${GLFW_SOURCE_DIR}/deps
    ${STB_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/ext/glm-master
)

# Ensure correct include paths
target_include_directories(lighting_test_v2 PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/common
    ${GLFW_SOURCE_DIR}/deps
    ${STB_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/ext/glm-master
)

# Ensure correct include paths
target_include_directories(snake_game PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/common
    ${GLFW_SOURCE_DIR}/deps
    ${STB_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/ext/glm-master
)

# Copy shaders only when they change
add_custom_command(TARGET opengl_window POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        ${CMAKE_BINARY_DIR}/shaders
    COMMENT "Copying shaders folder to build directory"
)

# Copy assets only when they change
add_custom_command(TARGET opengl_window POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/assets
    COMMENT "Copying assets folder to build directory"
)






