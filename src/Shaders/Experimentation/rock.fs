#version 330 core
out vec4 fragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_height1;
};
uniform Material material;

in vec2 tex;
in vec3 normal;
in vec3 fragPos;

void main() 
{
    vec4 diffuseMap = texture(material.texture_diffuse1, tex);
    vec4 specularMap = texture(material.texture_specular1, tex);
    vec4 heightMap = texture(material.texture_height1, tex);

    specularMap *= 0.1;
    if (diffuseMap.a < 0.05) discard;
    fragColor = vec4(heightMap.rgb, 1.0);
}