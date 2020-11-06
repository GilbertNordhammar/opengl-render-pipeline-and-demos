#version 330 core

#include <pipeline/lighting/Phong.glsl>
#include <pipeline/View.glsl>

in vec2 TexCoords;
in vec3 FragmentPosWS;
in vec3 Normal;

out vec4 FragColor;  

uniform bool enableSpecular;

void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(_viewPosWS - FragmentPosWS);
    
    vec4 diffuseMap = texture(_material_phong.texture_diffuse1, TexCoords);
    vec4 specularMap = enableSpecular ? 
        texture(_material_phong.texture_specular1, TexCoords)
        : vec4(0);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight_Phong(
        _dirLight_phong, 
        norm, 
        viewDir,
        _material_phong.shininess,
        diffuseMap,
        specularMap);

    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight_Phong(
            _pointLights_phong[i], 
            norm, 
            FragmentPosWS, 
            viewDir,
            _material_phong.shininess,
            diffuseMap,
            specularMap);    

    // phase 3: Spot light
    result += CalcSpotLight_Phong(
        _spotLight_phong,
        norm,
        FragmentPosWS,
        viewDir,
        _material_phong.shininess,
        diffuseMap,
        specularMap);    
    
    FragColor = vec4(result, diffuseMap.a);
}