#version 330 core

#include <pipeline/View.glsl>

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 model;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(_view * model)));
    vs_out.normal = vec3(vec4(normalMatrix * aNormal, 0.0));
    gl_Position = _view * model * vec4(aPos, 1.0); 
}