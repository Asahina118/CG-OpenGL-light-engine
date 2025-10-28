#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;        // passed from model * vertex_coor => interpolated as FragPos
in vec2 TexCoords;

struct Material {
    sampler2D diffuse; // diffuse map, same color as ambient.
    sampler2D specular;
    sampler2D emission;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;       // usually set to the light color
    vec3 specular;      // usually kept 1.0f

    // attenuation parameters : F_att := 1.0 / ( x_c + x_l * d + x_q * d^2 )
    vec3 attenuationParams;   // .x = constant term ; .y = linear term ; .z = quadratic term
};
uniform Light light;

struct Flashlight {
    int enableFlashlight;

    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cosCutOff;
    float outerCosCutOff;
    vec3 attenuationParams;
};
uniform Flashlight flashlight;

uniform vec3 cameraPos;

vec3 renderLight(vec3 lightPos, vec3 lightAmbient, vec3 lightDiffuse, vec3 lightSpecular, bool isFlashlight) {
    vec3 lightDir = normalize(lightPos - FragPos);

    vec3 textureDiffuse = vec3(texture(material.diffuse, TexCoords));
    vec3 textureSpecular = vec3(texture(material.specular, TexCoords));

    // light ambient
    vec3 ambient = lightAmbient * textureDiffuse;

    float intensity = 1.0f;
    if (isFlashlight) {
        // smaller cosTheta => closer to flashlight center
        float cosTheta = dot(lightDir, normalize(-flashlight.direction));
        if (cosTheta <= flashlight.outerCosCutOff) {
            // outside flashlight, return ambient directly
            // NOTE : for theta, cutOff \in (0, PI / 4), theta > cutOff <=> cosTheta < cosCutOff 
            return textureDiffuse * lightAmbient;
        }
        // if its inside the flashlight, all the below calculations are carried out as usual light rendering

        float epsilon = (flashlight.cosCutOff - flashlight.outerCosCutOff);
        intensity = clamp((cosTheta - flashlight.outerCosCutOff) / epsilon, 0.0, 1.0);
        // temporary fix
        // globalIntensity = intensity;
    }


    // diffuse
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0); // avoid negative dot product
    vec3 diffuse =  lightDiffuse * diff * textureDiffuse;

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    // NOTE : reflect function requires direction from light source. So we invert the lightDir 
    vec3 viewDir = normalize(cameraPos - FragPos);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightSpecular * (spec * textureSpecular);

    // attenuation
    float distance = length(lightPos - FragPos);
    float attenuation = 1.0 / (light.attenuationParams.x + light.attenuationParams.y * distance + light.attenuationParams.z * distance * distance);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    if (isFlashlight) {
        diffuse *= intensity;
        specular *= intensity;
    }

    vec3 result = ambient + diffuse + specular;

    return result;
}

void main() {
    vec3 flashlightResult;
    if (flashlight.enableFlashlight == 1) {
        flashlightResult = renderLight(flashlight.position, flashlight.ambient, flashlight.diffuse, flashlight.specular, true);

    } else {
        flashlightResult = vec3(0.0);
    }

    vec3 lightSourceResult = renderLight(light.position, light.ambient, light.diffuse, light.specular, false);


    // emission
    vec3 textureEmission = vec3(texture(material.emission, TexCoords));
    vec3 emission = textureEmission * vec3(0.1f);


    vec3 result = (flashlightResult + lightSourceResult) + emission ;

    FragColor = vec4(result, 1.0);
}