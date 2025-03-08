#version 330 core

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPosition;

in vec3 normal;
in vec3 fragPosition;

// The fragment shader runs once for each pixel on the screen.
// Here we only need a single output which is the color of the pixel.
void main() {
    // Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Normalize the interpolated normals
    vec3 norm = normalize(normal);

    // Diffuse lighting
    vec3 directionToLight = normalize(lightPosition - fragPosition);
    float diffuseStrength = max(dot(norm, directionToLight), 0.0);
    vec3 diffuse = diffuseStrength * lightColor;

    // Specular lighting
    float specularBaseStrength = 1.0;
    int shine = 256;
    vec3 directionToView = normalize(-fragPosition); // Note that the viewer is at (0, 0, 0)
    vec3 reflectedLightDirection = reflect(-directionToLight, norm);
    float specularStrength = pow(max(dot(directionToView, reflectedLightDirection), 0.0), shine);
    vec3 specular = specularStrength * specularBaseStrength * lightColor;

    // Final result
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
