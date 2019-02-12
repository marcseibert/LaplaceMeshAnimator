#version 330 core

in vec3 FragPosition;
in vec2 TexCoords;
in vec3 Normal;

uniform vec3 viewDir;

struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
};

struct light {
    vec3 lightPos;
    
    vec3 ambient;
    vec3 diffuse;
    
    vec3 specular;
};

uniform Material material;

out vec4 FragColor;

void main() {
    
    vec3 diffuse = viewDir * Normal;
    
    //vec3 specular = viewDir
    FragColor = vec4(diffuse,1) * 0.5 +  0.5 *texture(material.texture_diffuse1, TexCoords);//vec4(1,0,0,1);
    //FragColor = texture(material.texture_diffuse1, TexCoords);
}
