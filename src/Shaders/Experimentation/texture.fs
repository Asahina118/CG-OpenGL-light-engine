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
    fragColor = texture(material.texture_diffuse1, tex);
}