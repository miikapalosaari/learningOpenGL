#version 330 core
out vec4 FragColor;

in vec3 myColor;
in vec3 myPosition;

void main()
{
    FragColor = vec4(myPosition, 1.0f);
}