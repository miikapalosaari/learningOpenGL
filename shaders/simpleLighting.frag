#version 330 core
out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

in vec3 fragPos;
in vec3 normal;

void main() 
{
	// Ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;
	
	// Diffuse
	vec3 normalizedNormal = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diffuseMultiplier = max(dot(normalizedNormal, lightDir), 0.0);
	vec3 diffuse = diffuseMultiplier * lightColor;
	
	vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}


