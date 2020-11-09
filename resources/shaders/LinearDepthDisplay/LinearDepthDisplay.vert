#version 330 core

#include <pipeline/Mesh/vertexInput.glsl>
#include <pipeline/Common.glsl>

layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = _matrix_P * _matrix_V * _modelMatrix * vec4(aPos, 1.0f);
}