#version 330 core
out vec4 fragColor; 

in vecs {
    vec3 normal;
    vec2 tex;
    vec3 fragPos;
} fs_in;

uniform samplerCube skyBox;
uniform vec3 cameraPos;

void main() 
{
    // reflection  
    vec3 viewDir = normalize(fs_in.fragPos - cameraPos);
    vec3 reflectVec = reflect(viewDir, normalize(fs_in.normal));

    // refraction   
    float ratio = 1.00 / 1.52;
    vec3 refractVec = refract(viewDir, normalize(fs_in.normal), ratio);

    if (gl_FragCoord.x <= 960) {
        fragColor = vec4(texture(skyBox, reflectVec).rgb, 1.0);
    } else {
        fragColor = vec4(texture(skyBox, refractVec).rgb, 1.0);
    }

}