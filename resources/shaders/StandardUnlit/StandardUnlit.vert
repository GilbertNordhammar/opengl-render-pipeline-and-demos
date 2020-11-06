#version 330 core

#include <pipeline/Common.glsl>

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;
layout (location = 5) in mat4 aInstanceModel;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoord;

    mat4 modelMatrix = _instancingEnabled ? aInstanceModel : _modelMatrix;
    gl_Position = _projection * _view * modelMatrix * vec4(aPos, 1.0);
}