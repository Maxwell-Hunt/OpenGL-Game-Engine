#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 fragPosition;

// The vertex shader runs once per vertex, and its purpose is generally
// to transform 3d coordinates into other 3d coordinates
// The output of the vertex shader is given by the gl_Position variable which is a vec4

// Something common you might do in a vertex shader is normalize the data coming in to be
// between -1 and 1. Here our data is already in the desired form so we just pass it on directly
void main() {
    normal = aNormal;
    fragPosition = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
