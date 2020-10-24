#version 330 core

#include <global-data/View.glsl>

layout (location = 0) in vec3 aPos;

uniform mat4 model;

void main()
{
    gl_Position = rp_projection * rp_view * model * vec4(aPos, 1.0f);
}