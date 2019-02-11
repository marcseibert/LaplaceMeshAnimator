#version 330 core

in vec3 FragPosition;
in vec2 TexCoords;
in vec3 Normal;

uniform ivec4 code;
out vec4 FragColor;

void main() {
    FragColor = vec4(code.x / 255.0, code.y / 255.0, code.z / 255.0, code.w / 255.0);
}
