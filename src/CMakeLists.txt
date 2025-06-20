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

add_executable(maze_generation maze_generation.cpp ${GLAD_GL} ${COMMON_INC_FILES} ${COMMON_SRC_FILES})
target_link_libraries(maze_generation PRIVATE opengl_common)

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

# Ensure correct include paths
target_include_directories(maze_generation PUBLIC
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
add_dependencies(maze_generation copy_shaders copy_assets)


if(WIN32)
message(STATUS "Building on Windows")
message(STATUS "FFMPEG_LIBRARY_DIR = ${FFMPEG_LIBRARY_DIR}")
message(STATUS "FFMPEG_BIN_DIR = ${FFMPEG_BIN_DIR}")
message(STATUS "FFMPEG_LIBS = ${FFMPEG_LIBS}")

target_include_directories(ffmpeg_test PUBLIC ${FFMPEG_INCLUDE_DIR})
target_link_libraries(ffmpeg_test PRIVATE ${FFMPEG_LIBS})
target_link_directories(ffmpeg_test PRIVATE ${FFMPEG_LIBRARY_DIR})

add_custom_command(TARGET ffmpeg_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${FFMPEG_BIN_DIR}" $<TARGET_FILE_DIR:ffmpeg_test>
)
elseif(UNIX)
    message(STATUS "Building on Linux/Unix")
    message(STATUS "FFMPEG_LIBRARY_DIR = ${FFMPEG_LIBRARY_DIR}")
    message(STATUS "FFMPEG_LIBS = ${FFMPEG_LIBS}")

    target_include_directories(ffmpeg_test PUBLIC ${FFMPEG_INCLUDE_DIR})
    target_link_directories(ffmpeg_test PRIVATE ${FFMPEG_LIBRARY_DIR})
    target_link_libraries(ffmpeg_test PRIVATE ${FFMPEG_LIBS})

    set_target_properties(ffmpeg_test PROPERTIES
        BUILD_RPATH "${FFMPEG_LIBRARY_DIR}"
        INSTALL_RPATH "${FFMPEG_LIBRARY_DIR}"
    )

    get_target_property(BUILD_RPATH ffmpeg_test BUILD_RPATH)
    message(STATUS "Build RPATH of ffmpeg_test: ${BUILD_RPATH}")
endif()






