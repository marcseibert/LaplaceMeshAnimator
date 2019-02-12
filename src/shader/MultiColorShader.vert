#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

uniform mat4 mvpMatrix;
out vec4 Color;

void main() {
    gl_Position = mvpMatrix * vec4(aPos,1);
    Color = aColor;
}
