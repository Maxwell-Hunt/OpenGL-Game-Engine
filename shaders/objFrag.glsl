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

struct DirectionalLight {
    vec3 direction;

    vec3 color;
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
};

struct PointLight {
    vec3 position;
    
    vec3 color;
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;

    float kc;
    float kl;
    float kq;
};

#define NUM_POINT_LIGHTS 30

uniform Material material0;
uniform DirectionalLight skyLight;
uniform PointLight pointLights[NUM_POINT_LIGHTS];

uniform int numLights;
uniform vec3 color;

uniform bool usesTexture;

vec3 directionalLightContribution(DirectionalLight light) {
    vec3 diffuseColor = color;
    vec3 specularColor = color;
    if(usesTexture) {
        diffuseColor = vec3(texture(material0.diffuse, texCoords));
        specularColor = vec3(texture(material0.specular, texCoords));
    }

    // Ambient lighting
    vec3 ambient = diffuseColor * light.ambientStrength;

    // Normalize the interpolated normals
    vec3 norm = normalize(normal);

    // Diffuse lighting
    vec3 directionToLight = normalize(-light.direction);
    float diffuseStrength = max(dot(norm, directionToLight), 0.0);
    vec3 diffuse = diffuseStrength * diffuseColor * light.diffuseStrength;

    // Specular lighting
    vec3 directionToView = normalize(-fragPosition); // Note that the viewer is at (0, 0, 0)
    vec3 reflectedLightDirection = reflect(-directionToLight, norm);
    float specularStrength = pow(max(dot(directionToView, reflectedLightDirection), 0.0), material0.shine);
    vec3 specular = specularStrength * specularColor * light.specularStrength;
    return (ambient + diffuse + specular) * light.color;
}

vec3 pointLightContribution(PointLight light) {
    vec3 diffuseColor = color;
    vec3 specularColor = color;
    if(usesTexture) {
        diffuseColor = vec3(texture(material0.diffuse, texCoords));
        specularColor = vec3(texture(material0.specular, texCoords));
    }

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
    float specularStrength = pow(max(dot(directionToView, reflectedLightDirection), 0.0), material0.shine);
    vec3 specular = specularStrength * specularColor * light.specularStrength;

    // Attenuation
    float dist = length(fragPosition - light.position);
    float attenuation = 1.0 / (light.kc + light.kl * dist + light.kq * dist * dist);

    // Final result
    return attenuation * (ambient + diffuse + specular) * light.color;
}

// The fragment shader runs once for each pixel on the screen.
// Here we only need a single output which is the color of the pixel.
void main() {
    vec3 result = directionalLightContribution(skyLight);
    for(int i = 0;i < numLights;i++) {
        result += pointLightContribution(pointLights[i]);
    }
    FragColor = vec4(result, 1.0);
}
