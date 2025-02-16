#version 330 core
layout(location = 0) in vec3 position;  // Vertex position

// Uniforms for projection and view matrices
uniform mat4 projection;
uniform mat4 view;

void main() {
    // Apply the projection and view transformations
    gl_Position = projection * view * vec4(position, 1.0);
}


