#version 330 core

#include <pipeline/View.glsl>

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 matrix_V_noTranslation;

void main()
{
    TexCoords = aPos;
    vec4 pos = _projectionMatrix * matrix_V_noTranslation * vec4(aPos, 1.0);

    gl_Position = pos.xyww;
}  