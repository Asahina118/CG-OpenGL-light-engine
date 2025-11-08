#version 330 core
out vec4 fragColor;

in vec2 tex;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};
uniform Material material;

void main() 
{
    vec4 diffuseMap = texture(material.texture_diffuse1, tex);
    vec4 specularMap = texture(material.texture_specular1, tex);
    fragColor = diffuseMap + specularMap;
}