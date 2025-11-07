#version 330 core   
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vec3 normal;
out vec2 tex;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 trans;

void main() 
{
    vec4 modelPos = model*vec4(aPos, 1.0);
    gl_Position = trans * modelPos;
	normal = mat3(transpose(inverse(model))) * aNormal; 
    tex = aTex;
    fragPos = modelPos.xyz;
}
