#version 330 core

#include <pipeline/View.glsl>

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 5) in mat4 aInstanceModel;

uniform mat4 model;
uniform bool _instancingEnabled;

out vec2 TexCoords;
out vec3 FragmentPosWS;
out vec3 Normal;

void main()
{
    TexCoords = aTexCoord;
    
    mat4 modelMatrix = _instancingEnabled ? aInstanceModel : model;
    
    Normal = mat3(transpose(inverse(modelMatrix))) * aNormal; // TODO: Move transpose calculation to CPU and pass via uniform
    FragmentPosWS = vec3(modelMatrix * vec4(aPos, 1.0)); 
    gl_Position = _projection * _view * modelMatrix * vec4(aPos, 1.0);
}