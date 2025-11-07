#version 330 core
out vec4 fragColor; 

in vec3 normal;
in vec3 fragPos;

uniform samplerCube skyBox;
uniform vec3 cameraPos;

void main() 
{
    vec3 viewDir = normalize(fragPos - cameraPos);
    vec3 reflectVec = reflect(viewDir, normalize(normal));
    fragColor = vec4(texture(skyBox, reflectVec).rgb, 1.0);
}