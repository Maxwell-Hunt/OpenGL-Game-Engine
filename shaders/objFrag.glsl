#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec3 fragPosition;
in vec2 texCoords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shine;
};

struct Light {
    vec3 position;
    vec3 color;
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
};

uniform Material material;
uniform Light light;

// The fragment shader runs once for each pixel on the screen.
// Here we only need a single output which is the color of the pixel.
void main() {
    vec3 diffuseColor = vec3(texture(material.diffuse, texCoords));
    vec3 specularColor = vec3(texture(material.specular, texCoords));

    // Ambient lighting
    vec3 ambient = diffuseColor * light.ambientStrength;

    // Normalize the interpolated normals
    vec3 norm = normalize(normal);

    // Diffuse lighting
    vec3 directionToLight = normalize(light.position - fragPosition);
    float diffuseStrength = max(dot(norm, directionToLight), 0.0);
    vec3 diffuse = diffuseStrength * diffuseColor * light.diffuseStrength;

    // Specular lighting
    vec3 directionToView = normalize(-fragPosition); // Note that the viewer is at (0, 0, 0)
    vec3 reflectedLightDirection = reflect(-directionToLight, norm);
    float specularStrength = pow(max(dot(directionToView, reflectedLightDirection), 0.0), material.shine);
    vec3 specular = specularStrength * specularColor * light.specularStrength;

    // Final result
    vec3 result = (ambient + diffuse + specular) * light.color;
    FragColor = vec4(result, 1.0);
}
