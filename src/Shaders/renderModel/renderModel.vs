#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 normal;
out vec2 texCoords;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 transform; // = proj * view, model has too many vertices am thinking slow CPU calc >> fast GPU calc for high no. of times

void main() {
    vec4 modelPos = model * vec4(aPos, 1.0);
    gl_Position = transform * modelPos;
	normal = mat3(transpose(inverse(model))) * aNormal; // not fixing the model would make the backpack normal all reversed. Still havent figured out why yet.
    texCoords = aTexCoords;
    fragPos = modelPos.xyz;
}