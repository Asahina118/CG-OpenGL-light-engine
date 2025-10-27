#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material {
    sampler2D diffuse; // diffuse map, same color as ambient.

    vec3 specular;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 color;
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;       // usually set to the light color
    vec3 specular;      // usually kept 1.0f
};
uniform Light light;

uniform vec3 objectColor;
uniform vec3 cameraPos;

void main() {
    vec3 textureVec = vec3(texture(material.diffuse, TexCoords));

    // ambient
    vec3 ambient = light.ambient * textureVec;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0); // avoid negative dot product
    vec3 diffuse = diff * light.diffuse * textureVec;

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    // NOTE : reflect function requires direction from light source. So we invert the lightDir 
    vec3 viewDir = normalize(cameraPos - FragPos);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (spec * material.specular) * light.specular;

    // Combining
    vec3 result = (ambient + diffuse + specular) * light.color; // object color

    FragColor = vec4(result, 1.0);
}