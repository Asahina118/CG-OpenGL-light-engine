#version 330 core
out vec4 fragColor;

in vec3 fragPos;
in vec2 tex;
in vec3 normal;
in vec4 fragPosLight;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 attenuationParams;
};

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

    float shininess;
};

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform sampler2D shadowMap;

uniform vec3 viewPos;

float calcShadow(vec4 fragPosLight);

vec3 calcDirLight(DirLight dirLight, vec3 viewDir, vec3 normal, vec3 diffuseMap, vec3 specularMap);

vec3 calcPointLight(PointLight pointLight, vec3 viewDir, vec3 normal, vec3 fragPos, vec3 diffuseMap, vec3 specularMap);

void main() {
    vec4 diffuseMap = texture(material.texture_diffuse1, tex);
    vec3 specularMap = texture(material.texture_specular1, tex).rgb;
    if (diffuseMap.a < 0.1) discard;


    // diffuseMap *= textureTuning;
    // specularMap *= textureTuning;

    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 result = calcDirLight(dirLight, viewDir, norm, diffuseMap.rgb, specularMap);

    result += calcPointLight(pointLight, viewDir, norm, fragPos, diffuseMap.rgb, specularMap);

    fragColor = vec4(result, 1.0);
}

float calcShadow(vec4 fragPosLight)
{
    vec3 projCoords = fragPosLight.xyz / fragPosLight.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;

    float currentDepth = projCoords.z;

    float shadow = currentDepth > closestDepth ? 1.0 : 0.0; 

    return shadow;
}

vec3 calcDirLight(DirLight dirLight, vec3 viewDir, vec3 norm, vec3 diffuseMap, vec3 specularMap)
{
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(lightDir, norm), 0.0);

    vec3 halfwayDir = normalize(viewDir + lightDir );
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);

    vec3 ambient = dirLight.ambient * diffuseMap;
    vec3 diffuse = diff * dirLight.diffuse * diffuseMap;
    vec3 specular = spec * dirLight.specular * specularMap;

    // float shadow = calcShadow(fragPosLight);

    // vec3 result = diffuse + (1.0 - shadow)*(specular + ambient);

    vec3 result = diffuse + specular + ambient;
    return result;
}

vec3 calcPointLight(PointLight pointLight, vec3 viewDir, vec3 normal, vec3 fragPos, vec3 diffuseMap, vec3 specularMap)
{
    vec3 lightDir = normalize(pointLight.position - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfwayDir, normal), 0.0), material.shininess);

    vec3 diffuse = pointLight.diffuse * diffuseMap * diff;
    vec3 specular = pointLight.specular * specularMap * spec;
    vec3 ambient = pointLight.ambient * diffuseMap;

    float dist = length(fragPos - pointLight.position);
    float attenuation = 1.0 / (pointLight.attenuationParams.x + pointLight.attenuationParams.y * dist + pointLight.attenuationParams.z * dist * dist);

    diffuse *= attenuation;
    specular *= attenuation;
    ambient *= attenuation;

    float shadow = calcShadow(fragPosLight);
    vec3 result = ambient + (1.0 - shadow) * (diffuse + specular);
    return result;
}