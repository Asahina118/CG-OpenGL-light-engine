#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out VS_OUT{
    vec3 fragPos;
    vec3 normal;
    vec2 tex;
    vec4 fragPosLight;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 lightTrans;

uniform bool reverseNormal;

void main() 
{
    vec4 modelPos = model * vec4(aPos, 1.0);
    vs_out.fragPos = modelPos.xyz;
    vec3 normal = transpose(inverse(mat3(model))) * aNormal;
    if (reverseNormal) 
        vs_out.normal = -normal;
    else
        vs_out.normal = normal;


    vs_out.tex = aTex;
    vs_out.fragPosLight = lightTrans * modelPos;

    gl_Position = proj * view * modelPos;
}