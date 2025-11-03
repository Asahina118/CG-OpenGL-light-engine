#version 330 core
out vec4 fragColor;

in vec3 fragPos;
in vec2 texCoords;
in vec3 normal;

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

struct SpotLight {
    vec3 position;
    vec3 direction;

    float cosCutOff;
    float cosOuterCutOff;

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

#define MAX_POINT_LIGHTS 5
#define MAX_SPOTLIGHTS 5
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOTLIGHTS];
uniform DirLight dirLight;

uniform vec3 viewPos;
uniform int NR_POINT_LIGHTS;
uniform int NR_SPOTLIGHTS;
uniform bool enableFlashlight;

vec3 calcDirLight(DirLight dirLight, vec3 viewDir, vec3 normal, vec3 diffuseMap, vec3 specularMap);

vec3 calcPointLight(PointLight pointLight, vec3 viewDir, vec3 fragPos, vec3 normal, vec3 diffuseMap, vec3 specularMap);

vec3 calcSpotLight(SpotLight spotLight, vec3 viewDir, vec3 fragPos, vec3 normal, vec3 diffuseMap, vec3 specularMap);

void main() {
    vec3 diffuseMap = texture(material.texture_diffuse1, texCoords).rgb;
    vec3 specularMap = texture(material.texture_specular1, texCoords).rgb;
    vec3 normal = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 result = calcDirLight(dirLight, viewDir, normal, diffuseMap, specularMap);

    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += calcPointLight(pointLights[i], viewDir, fragPos, normal, diffuseMap, specularMap);

    }

    int startingIdx = (enableFlashlight)? 0 : 1;
    for (int i = startingIdx; i < NR_SPOTLIGHTS; i++) {
        result += calcSpotLight(spotLights[i], viewDir, fragPos, normal, diffuseMap, specularMap);
    }

    fragColor = vec4(result, 0.0);
}

vec3 calcDirLight(DirLight dirLight, vec3 viewDir, vec3 normal, vec3 diffuseMap, vec3 specularMap)
{
    vec3 lightDir = normalize(-dirLight.direction);
    float diffuse = max(dot(lightDir, normal), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float specular = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    
    vec3 ambientResult = dirLight.ambient * diffuseMap;
    vec3 diffuseResult = dirLight.diffuse * diffuseMap * diffuse;
    vec3 specularResult = dirLight.specular * specularMap * specular;

    vec3 result = ambientResult + diffuseResult + specularResult;
    return result;
}

vec3 calcPointLight(PointLight pointLight, vec3 viewDir, vec3 fragPos, vec3 normal, vec3 diffuseMap, vec3 specularMap) 
{
    vec3 lightDir = normalize(pointLight.position - fragPos);
    float diffuse = max(dot(lightDir, normal), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float specular = pow(max(dot(reflectDir, viewDir), 0.0), 32);

    vec3 ambientResult = pointLight.ambient * diffuseMap;
    vec3 diffuseResult = pointLight.diffuse * diffuse * diffuseMap;
    vec3 specularResult = pointLight.specular * specular * specularMap;

    //attenuation
    float distance = length(pointLight.position - fragPos);
    float attenuation = 1.0 / (pointLight.attenuationParams.x + pointLight.attenuationParams.y * distance + pointLight.attenuationParams.z * distance * distance);

    ambientResult *= attenuation;
    diffuseResult *= attenuation;
    specularResult *= attenuation;

    vec3 result = ambientResult + diffuseResult + specularResult;
    return result;
}

vec3 calcSpotLight(SpotLight spotLight, vec3 viewDir, vec3 fragPos, vec3 normal, vec3 diffuseMap, vec3 specularMap)
{
    vec3 lightDir = normalize(spotLight.position - fragPos);
    float diffuse = max(dot(lightDir, normal), 0.0);

    vec3 reflectionDir = normalize(reflect(-lightDir, normal));
    float specular = pow(max(dot(reflectionDir, viewDir), 0.0), material.shininess);

    //attenuation
    float distance = length(spotLight.position - fragPos);
    float attenuation = 1.0 / (spotLight.attenuationParams.x + spotLight.attenuationParams.y * distance + spotLight.attenuationParams.z * distance * distance);

    //intensity
    float cosTheta = dot(lightDir, normalize(-spotLight.direction));
    float epsilon = spotLight.cosCutOff - spotLight.cosOuterCutOff;
    float intensity = clamp((cosTheta - spotLight.cosOuterCutOff) / epsilon, 0.0, 1.0);
    
    vec3 ambientResult = spotLight.ambient * diffuseMap;
    vec3 diffuseResult = spotLight.diffuse * diffuseMap * diffuse;
    vec3 specularResult = spotLight.specular * specularMap * specular;

    ambientResult *= attenuation * intensity;
    diffuseResult *= attenuation * intensity;
    specularResult *= attenuation * intensity;

    vec3 result = ambientResult + diffuseResult + specularResult;
    return result;
}