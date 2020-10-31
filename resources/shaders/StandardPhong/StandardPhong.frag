#version 330 core

#include <PhongShading/Lighting.glsl>
#include <global-data/View.glsl>

in vec2 TexCoords;
in vec3 FragmentPosWS;
in vec3 Normal;

out vec4 FragColor;  

uniform bool enableSpecular;

void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(rp_viewPosWS - FragmentPosWS);
    
    vec4 diffuseMap = texture(material.texture_diffuse1, TexCoords);
    vec4 specularMap = enableSpecular ? 
        texture(material.texture_specular1, TexCoords)
        : vec4(0);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(
        dirLight, 
        norm, 
        viewDir,
        diffuseMap,
        specularMap);

    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(
            pointLights[i], 
            norm, 
            FragmentPosWS, 
            viewDir,
            diffuseMap,
            specularMap);    

    // phase 3: Spot light
    result += CalcSpotLight(
        spotLight,
        norm,
        FragmentPosWS,
        viewDir,
        diffuseMap,
        specularMap);    
    
    FragColor = vec4(result, diffuseMap.a);
//    FragColor = vec4(diffuseMap.rgb, 1.0);
}