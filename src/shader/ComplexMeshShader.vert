#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 mvpMatrix;

out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPosition;

void main() {
    gl_Position = mvpMatrix * vec4(aPos, 1);
    Normal = aNormal;
    TexCoords = aTexCoords;
    FragPosition = vec3(0,0,0);
    
}
