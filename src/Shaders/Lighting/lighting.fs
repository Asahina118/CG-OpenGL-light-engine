#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;        // passed from model * vertex_coor => interpolated as FragPos
in vec2 TexCoords;

struct Material {
    sampler2D diffuse; // diffuse map, same color as ambient.
    sampler2D specular;
    sampler2D emission;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 color;
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;       // usually set to the light color
    vec3 specular;      // usually kept 1.0f

    // attenuation parameters : F_att := 1.0 / ( x_c + x_l * d + x_q * d^2 )
    vec3 attenuationParams;   // .x = constant term ; .y = linear term ; .z = quadratic term
};
uniform Light light;

uniform vec3 cameraPos;

void main() {
    vec3 textureDiffuse = vec3(texture(material.diffuse, TexCoords));
    vec3 textureSpecular = vec3(texture(material.specular, TexCoords));
    vec3 textureEmission = vec3(texture(material.emission, TexCoords));

    // ambient
    vec3 ambient = light.ambient * textureDiffuse;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0); // avoid negative dot product
    vec3 diffuse =  light.diffuse * diff * textureDiffuse;

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    // NOTE : reflect function requires direction from light source. So we invert the lightDir 
    vec3 viewDir = normalize(cameraPos - FragPos);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * textureSpecular);

    //attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.attenuationParams.x + light.attenuationParams.y * distance + light.attenuationParams.z * distance * distance);

    //emission
    vec3 emission = textureEmission * vec3(0.1f);

    ambient *= attenuation;     // optional for global world light
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = (ambient + diffuse + specular) * light.color + emission;

    FragColor = vec4(result, 1.0);
}