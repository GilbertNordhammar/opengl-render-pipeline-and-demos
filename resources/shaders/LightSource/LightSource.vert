#version 330 core

#include <global-data/View.glsl>

layout (location = 0) in vec3 aPos;
layout (location = 5) in mat4 aInstanceModel;

uniform mat4 model;
uniform bool rp_instancingEnabled;

void main()
{
    mat4 modelMatrix = rp_instancingEnabled ? aInstanceModel : model;

    gl_Position = rp_projection * rp_view * modelMatrix * vec4(aPos, 1.0f);
}