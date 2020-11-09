#version 330 core

#include <pipeline/Mesh/vertexInput.glsl>
#include <pipeline/Common.glsl>

layout (location = 0) in vec3 _vPos;

void main()
{
    gl_Position = _matrix_P * _matrix_V * _modelMatrix * vec4(_vPos, 1.0f);
}