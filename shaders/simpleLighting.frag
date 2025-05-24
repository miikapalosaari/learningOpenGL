#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 lightPos;

in vec3 Normal;
in vec3 FragPos;

void main() 
{
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 normalizedNormal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diffuseMultiplier = max(dot(normalizedNormal, lightDir), 0.0);
    vec3 diffuse = diffuseMultiplier * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
	vec3 specular = vec3(0.0);
	
	if(diffuseMultiplier > 0.0)
	{
		vec3 reflectDir = reflect(-lightDir, normalizedNormal);  
		float specularIntensity = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		specular = specularStrength * specularIntensity * lightColor;
	}
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}


