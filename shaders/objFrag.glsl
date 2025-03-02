#version 330 core

out vec4 FragColor;

uniform vec4 objectColor;
uniform vec4 lightColor;

// The fragment shader runs once for each pixel on the screen.
// Here we only need a single output which is the color of the pixel.
void main() {
    FragColor = objectColor * lightColor;
    // FragColor = vec4(1.0f);
}
