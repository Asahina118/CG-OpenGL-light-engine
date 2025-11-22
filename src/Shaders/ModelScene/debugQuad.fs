#version 330 core
out vec4 fragColor;

in vec2 tex;

uniform sampler2D depthMap;
// uniform float nearPlane;
// uniform float farPlane;

// // for perspective matrix
// float linearizeDepth(float depth)
// {
//     float z = depth * 2.0 - 1.0; // Back to NDC 
//     return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));	
// }


void main()
{
    float depthValue = texture(depthMap, tex).r;

    // fragColor = vec4(vec3(linearizeDepth(depthValue) / farPlane), 1.0);
    fragColor = vec4(vec3(depthValue), 1.0);
}