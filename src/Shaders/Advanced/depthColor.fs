#version 330 core
out vec4 fragColor;

in vec2 texCoords;

struct Material {
    sampler2D texture_diffuse1;
};
uniform Material material;

float near = 0.1;
float far = 7.0;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main() 
{
    vec4 diffuseMap = texture(material.texture_diffuse1, texCoords);
    fragColor = diffuseMap;

    float depth = LinearizeDepth(gl_FragCoord.z) / far;
    fragColor = vec4(vec3(depth), 1.0);
}
