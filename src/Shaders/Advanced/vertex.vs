#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 texCoords;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 trans;

void main() {
    vec4 modelPos = model * vec4(aPos, 1.0);
    gl_Position = trans * modelPos;
	normal = mat3(transpose(inverse(model))) * aNormal;
    texCoords = aTexCoords;
    fragPos = modelPos.xyz;
}