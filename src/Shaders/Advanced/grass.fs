#version 330 core
out vec4 fragColor;

in vec2 texCoords;

struct Material {
    sampler2D texture_diffuse1;
};
uniform Material material;

void main() 
{
    vec4 diffuseMap = texture(material.texture_diffuse1, texCoords);
    if (diffuseMap.a < 0.1) discard;
    fragColor = diffuseMap;
}