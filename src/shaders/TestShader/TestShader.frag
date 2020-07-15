#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoord;
in vec3 WorldPos;
in vec3 Normal;

uniform sampler2D texture1;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

out vec4 FragColor;  

void main()
{
    vec3 ambient = light.ambient * material.ambient;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - WorldPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // Phong lighting (TODO: Adding alternative Blinn-Phong?)
    vec3 viewDir = normalize(viewPos - WorldPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float specularStrength = 0.5;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 col = (ambient + diffuse) * texture(texture1, TexCoord).rgb + specular;
    FragColor = vec4(col, 1.0);
}