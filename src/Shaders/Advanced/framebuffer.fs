#version 330 core
out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D screenTexture;

uniform int choice;

vec4 calcInversion(vec4 diffuseMap)
{
    return vec4(vec3(1.0 - diffuseMap), 1.0);
}

vec4 calcGreyScale(vec4 diffuseMap) 
{
    // weighting according to human eye color sensitivity
    vec3 weighting = vec3(0.2126, 0.7152, 0.0722);
    float average = dot(weighting, diffuseMap.rgb);
    return vec4(average, average, average, 1.0);
}

vec4 calcKernelEffects(float kernel[9])
{
    const float offset = 1.0 / 300.0;
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset),
        vec2( 0.0f,    offset),
        vec2( offset,  offset),
        vec2(-offset,  0.0f), 
        vec2( 0.0f,    0.0f),
        vec2( offset,  0.0f),
        vec2(-offset, -offset),
        vec2( 0.0f,   -offset),
        vec2( offset, -offset)
    );

    vec3 sampleTex[9];
    for (int i = 0; i < 9; i++) {
        sampleTex[i] = vec3(texture(screenTexture, texCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; i++) {
        col += sampleTex[i] * kernel[i];
    }

    return vec4(col, 1.0);
}

void main() 
{
    vec4 diffuseMap = texture(screenTexture, texCoords);

    if (choice == 1)
        fragColor = calcInversion(diffuseMap);
    else if (choice == 2)
        fragColor = calcGreyScale(diffuseMap);
    else if (choice == 3) {
        float kernel[9] = float[] (
            -1, -1, -1,
            -1, 9, -1,
            -1, -1, -1
        );
        fragColor = calcKernelEffects(kernel);
    }
    else if (choice == 4) {
        float kernel[9] = float[] (
            1.0 / 16, 2.0 / 16, 1.0 / 16,
            2.0 / 16, 4.0 / 16, 2.0 / 16,
            1.0 / 16, 2.0 / 16, 1.0 / 16
        );
        fragColor = calcKernelEffects(kernel);
    }
    else
        fragColor = vec4(diffuseMap.rgb, 1.0);
}