struct Material {
    float shininess;

    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NR_POINT_LIGHTS 4

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

vec3 CalcDirLight(
    DirLight light, 
    vec3 normal, 
    vec3 viewDir,
    vec4 diffuseMap,
    vec4 specularMap
    )
{
    vec3 lightDir = normalize(-light.direction);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // combine results
    vec3 ambient  = light.ambient  * diffuseMap.rgb * diffuseMap.a;
    vec3 diffuse  = light.diffuse  * diff * diffuseMap.rgb * diffuseMap.a;
    vec3 specular = light.specular * spec * specularMap.rgb;
    return max(vec3(0), ambient + diffuse + specular);
}

vec3 CalcPointLight(
    PointLight light, 
    vec3 normal, 
    vec3 fragPos, 
    vec3 viewDir,
    vec4 diffuseMap,
    vec4 specularMap
    )
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    

    // combine results
    vec3 ambient  = light.ambient  * diffuseMap.rgb * diffuseMap.a;
    vec3 diffuse  = light.diffuse  * diff * diffuseMap.rgb * diffuseMap.a;
    vec3 specular = light.specular * spec * specularMap.rgb;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return max(vec3(0), ambient + diffuse + specular);
}

vec3 CalcSpotLight(
    SpotLight light, 
    vec3 normal, 
    vec3 fragPos, 
    vec3 viewDir,
    vec4 diffuseMap,
    vec4 specularMap
    )
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // combine results
    vec3 ambient = light.ambient * diffuseMap.rgb * diffuseMap.a;
    vec3 diffuse = light.diffuse * diff * diffuseMap.rgb * diffuseMap.a;
    vec3 specular = light.specular * spec * specularMap.rgb;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return max(vec3(0), ambient + diffuse + specular);
}