#version 330 core

#include <pipeline/Mesh/vertexInput.glsl>
#include <pipeline/Common.glsl>

out vec2 TexCoords;
out vec3 FragmentPosWS;
out vec3 Normal;

void main()
{
    TexCoords = aTexCoord;
    
    mat4 modelMatrix = _instancingEnabled ? aInstanceModel : _modelMatrix;
    
    Normal = mat3(transpose(inverse(modelMatrix))) * aNormal; // TODO: Move transpose calculation to CPU and pass via uniform
    FragmentPosWS = vec3(modelMatrix * vec4(aPos, 1.0)); 
    gl_Position = _matrix_P * _matrix_V * modelMatrix * vec4(aPos, 1.0);
}