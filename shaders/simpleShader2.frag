#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D myTexture1;
uniform sampler2D myTexture2;
//uniform float visibility;

void main()
{
    FragColor = mix(texture(myTexture1, TexCoord), texture(myTexture2, vec2(-TexCoord.x, TexCoord.y)), 0.2f);
}