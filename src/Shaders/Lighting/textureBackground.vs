#version 330 core
layout(location = 0) in vec3 aPos;
layout (location =1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 mat;
uniform float size;

void main() {
	gl_Position = mat * vec4(size * aPos, 1.0);
	TexCoord = aTexCoord;
}
