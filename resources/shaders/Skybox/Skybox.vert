#version 330 core

#include <pipeline/View.glsl>

layout (location = 0) in vec3 vPos;

out vec3 TexCoords;

uniform mat4 matrix_V_noTranslation;

void main()
{
    TexCoords = vPos;
    vec4 pos = _matrix_P * matrix_V_noTranslation * vec4(vPos, 1.0);

    gl_Position = pos.xyww;
}  