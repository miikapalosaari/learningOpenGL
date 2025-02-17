#version 330 core
layout(location = 0) in vec3 position;  // Vertex position
layout(location = 1) in vec3 color;

// Uniforms for projection and view matrices
uniform mat4 projection;
uniform mat4 view;

out vec3 vColor;

void main() {
    // Apply the projection and view transformations
    gl_Position = projection * view * vec4(position, 1.0);
	
	vColor = color;
}


