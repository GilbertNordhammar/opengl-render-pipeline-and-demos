#version 330 core

#include <global-data/View.glsl>

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoord;
    gl_Position = rp_projection * rp_view * model * vec4(aPos, 1.0);
}