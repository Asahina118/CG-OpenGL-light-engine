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
uniform samplerCube shadowCubeMap;

uniform vec3 viewPos;
uniform float farPlane;

vec3 calcPointLight(PointLight pointLight, vec3 viewDir, vec3 normal, vec3 fragPos, vec3 diffuseMap, vec3 specularMap, vec4 fragPosLight);

float calcShadow(vec4 fragPosLight);

float calcPointShadow(PointLight pointLight, vec3 fragPos);

void main()
{
    vec4 diffuseMap = texture(material.texture_diffuse1, fs_in.tex);
    vec4 specularMap = texture(material.texture_specular1, fs_in.tex);
    if (diffuseMap.a < 0.1) discard;

    vec3 viewDir = normalize(viewPos - fs_in.fragPos);
    vec3 normal = normalize(fs_in.normal);

    vec3 result = calcPointLight(pointLight, viewDir, normal, fs_in.fragPos, diffuseMap.rgb, specularMap.rgb, fs_in.fragPosLight);


    fragColor = vec4(result, 1.0);
    // fragColor = vec4(texture(material.texture_diffuse1, fs_in.tex));

    // vec3 fragToLight = fs_in.fragPos - pointLight.position;
    // float closestDepth = texture(shadowCubeMap, fragToLight).r;
    // fragColor = vec4(vec3(closestDepth / farPlane), 1.0);
}

float calcShadow(vec4 fragPosLight)
{
    // NDC
    vec3 projCoords = fragPosLight.xyz / fragPosLight.w;
    projCoords = projCoords * 0.5 + 0.5;
    if (projCoords.z > 1.0) 
        return 0.0;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

float calcShadowPCF(vec4 fragPosLight)
{
    vec3 projCoords = fragPosLight.xyz / fragPosLight.w;
    projCoords = projCoords * 0.5 + 0.5;
    float shadow = 0.0;

    if (projCoords.z > 1.0) 
        return 0.0;

    float currentDepth = projCoords.z;
    float bias = 0.005;

    vec2 texSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texSize).r;
            shadow += currentDepth - bias > pcfDepth? 1.0: 0.0;
        }
    }

    shadow /= 9.0;
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

    // float shadow = calcShadow(fragPosLight);
    // float shadow = calcShadowPCF(fragPosLight);
    float shadow = calcPointShadow(pointLight, fragPos);

    vec3 result = ambient + (1.0 - shadow) * (diffuse + specular);

    return result;
}

float calcPointShadow(PointLight pointLight, vec3 fragPos)
{
    vec3 fragToLight = fragPos - pointLight.position;
    float closestDepth = texture(shadowCubeMap, fragToLight).r;
    closestDepth *= farPlane;

    float currentDepth = length(fragToLight);
    float bias = 0.05;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}