#version 330 core

#include <pipeline/Mesh/vertexInput.glsl>
#include <pipeline/Common.glsl>

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 matrix_M;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(_matrix_V * matrix_M)));
    vs_out.normal = vec3(vec4(normalMatrix * _vNormal, 0.0));
    gl_Position = _matrix_V * matrix_M * vec4(_vPos, 1.0); 
}