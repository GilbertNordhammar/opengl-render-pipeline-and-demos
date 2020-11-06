#version 330 core

#include <pipeline/View.glsl>

layout (location = 0) in vec3 aPos;
layout (location = 5) in mat4 aInstanceModel;

uniform mat4 model;
uniform bool _instancingEnabled;

void main()
{
    mat4 modelMatrix = _instancingEnabled ? aInstanceModel : model;

    gl_Position = _projection * _view * modelMatrix * vec4(aPos, 1.0f);
}