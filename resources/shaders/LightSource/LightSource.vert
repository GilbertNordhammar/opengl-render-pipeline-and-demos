#version 330 core

#include <pipeline/Common.glsl>

layout (location = 0) in vec3 aPos;
layout (location = 5) in mat4 aInstanceModel;

void main()
{
    mat4 modelMatrix = _instancingEnabled ? aInstanceModel : _modelMatrix;

    gl_Position = _projectionMatrix * _viewMatrix * modelMatrix * vec4(aPos, 1.0f);
}