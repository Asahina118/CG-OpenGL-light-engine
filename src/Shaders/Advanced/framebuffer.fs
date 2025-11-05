#version 330 core
out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D screenTexture;

uniform bool inversion;
uniform bool greyScale;
uniform bool kernelEffects;

vec4 calcInversion(vec3 diffuseMap)
{
    return vec4(vec3(1.0 - diffuseMap), 1.0);
}

vec4 calcGreyScale(vec3 diffuseMap) 
{
    // weighting according to human eye color sensitivity
    vec3 weighting = vec3(0.2126, 0.7152, 0.0722);
    float average = dot(weighting, diffuseMap);
    return vec4(average, average, average, 1.0);
}

vec4 calcKernelEffects(vec3 diffuseMap)
{
    return vec4(0);
}

void main() 
{
    vec3 diffuseMap = texture(screenTexture, texCoords).rgb;

    int count = 0;
    vec4 result = vec4(diffuseMap, 1.0);
    if (inversion) {
        result = calcInversion(diffuseMap);
        count++;
    } 

    if (greyScale) {
        result = (result * count + calcGreyScale(diffuseMap) ) / (count + 1);
        count++;
    } 

    if (kernelEffects) {
        result = (result * count + calcKernelEffects(diffuseMap) ) / (count + 1);
        count++;
    }

    fragColor = result;
    // fragColor = vec4(text.rgb, 1.0);
}