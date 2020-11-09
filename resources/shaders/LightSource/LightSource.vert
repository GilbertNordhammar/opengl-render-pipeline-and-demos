#version 330 core

#include <pipeline/Mesh/vertexInput.glsl>
#include <pipeline/Common.glsl>

void main()
{
    mat4 modelMatrix = _instancingEnabled ? _vInstanceModel : _modelMatrix;

    gl_Position = _matrix_P * _matrix_V * modelMatrix * vec4(_vPos, 1.0f);
}