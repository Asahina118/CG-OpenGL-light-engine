#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float size;

void main() {
	gl_Position = proj * view * model * vec4(size * aPos, 1.0);

	Normal = mat3(transpose(inverse(model))) * aNormal; // correction of non uniform effects on the relative angel between normal and the surface.

	FragPos = vec3(model * vec4(aPos, 1.0));
}