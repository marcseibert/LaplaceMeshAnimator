#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 Normal;
uniform mat4 mvpMatrix;

void main() {
    gl_Position = mvpMatrix * vec4(aPos,1);
    Normal = aNormal;
    //gl_Position = vec4(aPos, 1);
}
