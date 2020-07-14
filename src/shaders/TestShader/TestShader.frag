#version 330 core

in vec2 TexCoord;
in vec3 WorldPos;
in vec3 Normal;

uniform sampler2D texture1;
uniform vec3 lightColor;
uniform vec3 lightPos;

out vec4 FragColor;  

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - WorldPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 col = (ambient + diffuse) * texture(texture1, TexCoord).rgb;
    FragColor = vec4(col, 1.0);
}