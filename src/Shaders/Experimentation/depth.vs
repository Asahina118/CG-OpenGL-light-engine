#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightTrans;
uniform mat4 model;

void main()
{
    gl_Position = lightTrans * model * vec4(aPos, 1.0);
}