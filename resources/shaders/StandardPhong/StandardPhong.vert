#version 330 core

#include <pipeline/Mesh/vertexInput.glsl>
#include <pipeline/Common.glsl>

out vec2 TexCoords;
out vec3 FragmentPosWS;
out vec3 Normal;

void main()
{
    TexCoords = _vTexCoord;
    
    mat4 modelMatrix = _instancingEnabled ? _vInstanceModel : _modelMatrix;
    
    Normal = mat3(transpose(inverse(modelMatrix))) * _vNormal; // TODO: Move transpose calculation to CPU and pass via uniform
    FragmentPosWS = vec3(modelMatrix * vec4(_vPos, 1.0)); 
    gl_Position = _matrix_P * _matrix_V * modelMatrix * vec4(_vPos, 1.0);
}