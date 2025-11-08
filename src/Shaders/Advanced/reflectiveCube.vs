#version 330 core   
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vecs {
    vec3 normal;
    vec2 tex;
    vec3 fragPos;
} vs_out;

uniform mat4 model;
uniform mat4 trans;

void main() 
{
    vec4 modelPos = model*vec4(aPos, 1.0);
    gl_Position = trans * modelPos;
	vs_out.normal = mat3(transpose(inverse(model))) * aNormal; 
    vs_out.tex = aTex;
    vs_out.fragPos = modelPos.xyz;
}
