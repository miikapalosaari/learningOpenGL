#version 330 core
out vec4 FragColor;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;

void main() 
{
    vec3 ambient = light.ambient * material.ambient;
  	
    // diffuse 
    vec3 normalizedNormal = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diffuseMultiplier = max(dot(normalizedNormal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diffuseMultiplier * material.diffuse);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
	vec3 specular = vec3(0.0);
	
	if(diffuseMultiplier > 0.0)
	{
		vec3 reflectDir = reflect(-lightDir, normalizedNormal);  
		float specularIntensity = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		specular = light.specular * (specularIntensity * material.specular);
	}
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}