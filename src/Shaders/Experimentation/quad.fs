#version 330 core
out vec4 fragColor;

in vec2 tex;

uniform sampler2D depthMap;

void main() 
{
    float depthValue = texture(depthMap, tex).r;
    // fragColor = vec4(vec3(depthValue ), 1.0);

    fragColor = vec4(texture(depthMap, tex));
}