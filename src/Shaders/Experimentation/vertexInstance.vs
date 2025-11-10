#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTex;
layout (location = 3) in mat4 instanceMat;

out vec2 tex;

uniform mat4 view;
uniform mat4 proj;

void main() 
{
    gl_Position = proj * view * instanceMat * vec4(aPos, 1.0);
    tex = aTex;
}