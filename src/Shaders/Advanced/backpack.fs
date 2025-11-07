#version 330 core   
out vec4 fragColor;

in vec3 normal;
in vec3 fragPos;

uniform vec3 cameraPos;

struct Material {
    samplerCube texture_diffuse1;
};
uniform Material material;

void main() 
{
    vec3 viewVec = normalize(fragPos - cameraPos);
    vec3 textureVec = reflect(viewVec, normalize(normal));
    fragColor = vec4(texture(material.texture_diffuse1, textureVec).rgb, 1.0);
}