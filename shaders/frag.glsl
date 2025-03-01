#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D crateTexture;
uniform sampler2D faceTexture;

// The fragment shader runs once for each pixel on the screen.
// Here we only need a single output which is the color of the pixel.
void main() {
    FragColor = mix(texture(crateTexture, texCoord), texture(faceTexture, texCoord), 0.2);
}
