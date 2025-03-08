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

    // Diffuse lighting
    vec3 norm = normalize(normal);
    vec3 reverseLightDir = normalize(lightPosition - fragPosition);
    float diffuseStrength = max(dot(norm, reverseLightDir), 0.0);
    vec3 diffuse = diffuseStrength * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}
