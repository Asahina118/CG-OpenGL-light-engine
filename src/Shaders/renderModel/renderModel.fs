#version 330 core
out vec4 FragColor;

in vec2 texCoords;
in vec3 normal;

struct Material{
    sampler2D texture_diffuse1;
};
uniform Material material;

uniform sampler2D texture_diffuse1;

void main() {
    vec3 result;
    FragColor = texture(material.texture_diffuse1, texCoords);
}