#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;

void main() {
    vec3 dimStrength = vec3(0.5, 0.5, 0.5);
    vec4 tex = texture(texture1, TexCoord);
    FragColor = vec4(tex.rgb * dimStrength, tex.a);
}