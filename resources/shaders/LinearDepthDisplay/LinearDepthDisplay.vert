#version 330 core

#include <pipeline/Common.glsl>

layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = _projection * _view * _modelMatrix * vec4(aPos, 1.0f);
}