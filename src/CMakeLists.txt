message(STATUS "FFMPEG_LIBRARY_DIR = ${FFMPEG_LIBRARY_DIR}")
message(STATUS "FFMPEG_BIN_DIR = ${FFMPEG_BIN_DIR}")
message(STATUS "FFMPEG_LIBS = ${FFMPEG_LIBS}")

include_directories("${GLFW_SOURCE_DIR}/deps")

file(GLOB_RECURSE COMMON_INC_FILES "${CMAKE_SOURCE_DIR}/include/common/*.h")
file(GLOB_RECURSE COMMON_SRC_FILES "${CMAKE_SOURCE_DIR}/src/common/*.cpp")

set(GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/glad.c")

add_executable(opengl_window main_opengl_window.cpp ${GLAD_GL})
target_link_libraries(opengl_window PRIVATE opengl_common)

add_executable(shader_class_v1 main_shader_class_v1.cpp ${GLAD_GL} ${COMMON_INC_FILES} ${COMMON_SRC_FILES})
target_link_libraries(shader_class_v1 PRIVATE opengl_common)

add_executable(shader_class_v2 main_shader_class_v2.cpp ${GLAD_GL} ${COMMON_INC_FILES} ${COMMON_SRC_FILES})
target_link_libraries(shader_class_v2 PRIVATE opengl_common)

add_executable(spritebatch_test main_spritebatch_test.cpp ${GLAD_GL} ${COMMON_INC_FILES} ${COMMON_SRC_FILES})
target_link_libraries(spritebatch_test PRIVATE opengl_common)

add_executable(lighting_test_v1 main_lighting_test_v1.cpp ${GLAD_GL} ${COMMON_INC_FILES} ${COMMON_SRC_FILES})
target_link_libraries(lighting_test_v1 PRIVATE opengl_common)

add_executable(lighting_test_v2 main_lighting_test_v2.cpp ${GLAD_GL} ${COMMON_INC_FILES} ${COMMON_SRC_FILES})
target_link_libraries(lighting_test_v2 PRIVATE opengl_common)

add_executable(snake_game snake_game.cpp ${GLAD_GL} ${COMMON_INC_FILES} ${COMMON_SRC_FILES})
target_link_libraries(snake_game PRIVATE opengl_common)

add_executable(ffmpeg_test ffmpeg_test.cpp ${GLAD_GL} ${COMMON_INC_FILES} ${COMMON_SRC_FILES})
target_link_libraries(ffmpeg_test PRIVATE opengl_common)

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

# Ensure correct include paths
target_include_directories(ffmpeg_test PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/common
    ${GLFW_SOURCE_DIR}/deps
    ${STB_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/ext/glm-master
    ${FFMPEG_INCLUDE_DIR}
)

# Always copy shaders
add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        ${CMAKE_BINARY_DIR}/shaders
    COMMENT "Copying shaders folder to build directory"
)

# Always copy assets
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/assets
    COMMENT "Copying assets folder to build directory"
)


add_dependencies(opengl_window copy_shaders copy_assets)
add_dependencies(shader_class_v1 copy_shaders copy_assets)
add_dependencies(shader_class_v2 copy_shaders copy_assets)
add_dependencies(spritebatch_test copy_shaders copy_assets)
add_dependencies(lighting_test_v1 copy_shaders copy_assets)
add_dependencies(lighting_test_v2 copy_shaders copy_assets)
add_dependencies(snake_game copy_shaders copy_assets)
add_dependencies(ffmpeg_test copy_shaders copy_assets)


target_link_libraries(ffmpeg_test PRIVATE ${FFMPEG_LIBS})
target_link_directories(ffmpeg_test PRIVATE ${FFMPEG_LIBRARY_DIR})


add_custom_command(TARGET ffmpeg_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${FFMPEG_BIN_DIR}" $<TARGET_FILE_DIR:ffmpeg_test>
)






