#version 330 core

in vec3 Normal;
out vec4 FragColor;

uniform vec4 color;

uniform float diffuseStrength;
uniform vec3 viewDir;

void main() {
    float diffuse = dot(Normal, viewDir) * (diffuseStrength) + (1-diffuseStrength);
    FragColor = color * diffuse;
}
