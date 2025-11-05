#version 330 core
out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D screenTexture;

void main() {
    vec4 text = texture(screenTexture, texCoords);
    fragColor = vec4(vec3(1.0 - text), 1.0);
    // fragColor = vec4(text.rgb, 1.0);
}