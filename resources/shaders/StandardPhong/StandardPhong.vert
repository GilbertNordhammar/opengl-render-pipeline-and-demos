#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;
out vec3 FragmentPosWS;
out vec3 Normal;

void main()
{
    TexCoords = aTexCoord;
    FragmentPosWS = vec3(model * vec4(aPos, 1.0)); 
    Normal = mat3(transpose(inverse(model))) * aNormal; // TODO: Move transpose calculation to CPU and pass via uniform
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}