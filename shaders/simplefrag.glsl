#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D textureDiffuse0;

void main() {
    FragColor = texture(textureDiffuse0, texCoords);
}
