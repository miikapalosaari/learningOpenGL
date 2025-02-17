#version 330 core
out vec4 FragColor;
in vec3 vColor;

void main() 
{
    // Set the color of the plane to red
    FragColor = vec4(vColor, 1.0);  // RGBA: Red, Green, Blue, Alpha
}


