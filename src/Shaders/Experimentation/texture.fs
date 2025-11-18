#version 330 core
out vec4 fragColor;

in VS_OUT{
    vec3 fragPos;
    vec3 normal;
    vec2 tex;
    vec4 fragPosLight;
} fs_in;

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

uniform PointLight pointLight;
uniform Material material;

uniform sampler2D shadowMap;

uniform vec3 viewPos;

vec3 calcPointLight(PointLight pointLight, vec3 viewDir, vec3 normal, vec3 fragPos, vec3 diffuseMap, vec3 specularMap, vec4 fragPosLight);
float calcShadow(vec4 fragPosLight);

void main()
{
    vec4 diffuseMap = texture(material.texture_diffuse1, fs_in.tex);
    vec4 specularMap = texture(material.texture_specular1, fs_in.tex);
    if (diffuseMap.a < 0.1) discard;

    vec3 viewDir = normalize(viewPos - fs_in.fragPos);
    vec3 normal = normalize(fs_in.normal);

    vec3 result = calcPointLight(pointLight, viewDir, normal, fs_in.fragPos, diffuseMap.rgb, specularMap.rgb, fs_in.fragPosLight);


    fragColor = vec4(result, 1.0);
}

float calcShadow(vec4 fragPosLight)
{
    // NDC
    vec3 projCoords = fragPosLight.xyz / fragPosLight.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

    return shadow;
}

vec3 calcPointLight(PointLight pointLight, vec3 viewDir, vec3 normal, vec3 fragPos, vec3 diffuseMap, vec3 specularMap, vec4 fragPosLight) 
{
    vec3 lightDir = normalize(pointLight.position - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);

    vec3 halfwayDir = normalize(viewDir + lightDir);
    float spec = pow(max(dot(halfwayDir, normal), 0.0), 128.0);

    vec3 ambient = pointLight.ambient * diffuseMap;
    vec3 diffuse = pointLight.diffuse * diffuseMap * diff;
    vec3 specular = pointLight.specular * specularMap * spec;

    float shadow = calcShadow(fragPosLight);

    vec3 result = ambient + (1.0 - shadow) * (diffuse + specular);

    return result;
}