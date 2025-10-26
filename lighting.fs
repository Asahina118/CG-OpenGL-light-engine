#version 330 core
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform int shininess;

uniform vec3 cameraPos;
uniform float ambientStrength;
uniform float specularStrength;

void main() {
    // ambient
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0); // avoid negative dot product
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    // NOTE : reflect function requires direction from light source. So we invert the lightDir 

    vec3 viewDir = normalize(cameraPos - FragPos);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    // Combining
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}