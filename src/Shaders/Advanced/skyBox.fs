#version 330 core
out vec4 fragColor;

in vec3 texCoords;

uniform samplerCube skyBoxTexture;

void main() 
{
    fragColor = texture(skyBoxTexture, texCoords);
}