#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform bool usesTexture;
uniform sampler2D textureDiffuse0;
uniform vec3 color;

void main() {
    vec4 result = vec4(color, 1.0);
    if(usesTexture) {
        result = texture(textureDiffuse0, texCoords);
    }
    FragColor = result;
}
