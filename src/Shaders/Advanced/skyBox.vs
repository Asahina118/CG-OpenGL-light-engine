#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 texCoords;

uniform mat4 view;
uniform mat4 proj;

void main()
{
    mat4 view = mat4(mat3(view));
    gl_Position = proj * view * vec4(aPos, 1.0);
    texCoords = aPos;
}