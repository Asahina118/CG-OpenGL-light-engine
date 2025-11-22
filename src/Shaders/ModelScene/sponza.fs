#version 330 core
out vec4 fragColor;

in vec3 fragPos;
in vec2 tex;
in vec3 normal;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

    float shininess;
};

uniform Material material;

uniform DirLight dirLight;

uniform float textureTuning;

uniform vec3 viewPos;

vec3 calcDirLight(DirLight dirLight, vec3 viewDir, vec3 normal, vec3 diffuseMap, vec3 specularMap);

void main() {
    vec4 diffuseMap = texture(material.texture_diffuse1, tex);
    vec3 specularMap = texture(material.texture_specular1, tex).rgb;
    if (diffuseMap.a < 0.1) discard;

    diffuseMap *= textureTuning;
    specularMap *= textureTuning;

    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 result = calcDirLight(dirLight, viewDir, norm, diffuseMap.rgb, specularMap);

    fragColor = vec4(result, 1.0);
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

    vec3 result = diffuse + specular + ambient;
    return result;
}
