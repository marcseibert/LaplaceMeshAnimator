#include "Util.h"
#include <string>
#include <iostream>
#include "../extern/stb_image.h"

GLchar* loadShaderFromFile(const char* path) {
    
    std::ifstream in(path);
    
    std::string tempString = std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    
    char* cstr = new char[tempString.length() + 1];
    std::strcpy(cstr, tempString.c_str());
    
    return cstr;
}

unsigned int TextureFromFile(std::string const &filename, bool gamma)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = 0;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
    }
    
    //std::cout << " LOADED IMAGE " << width << "x" << height << " " << textureID <<std::endl;
    
    return textureID;
}
unsigned int uiTranslateColorCode(glm::vec4 colorCode) {
    return ((unsigned int)colorCode.y + (unsigned int)colorCode.x * 255) * 255 + (unsigned int)colorCode.z;
};

glm::ivec4 TranslateToColorCode(unsigned int code) {
    glm::ivec4 colorCode(1);
    if(code >= 255 * 255 * 255) {
        std::cout << "ERROR: Color Code Range exceeded! " << code << std::endl;
        return glm::ivec4(1, 0, 1, 1);
    }

    colorCode.z = code % 255;
    code = code / 255;

    colorCode.y = code % 255;
    code = code / 255;

    colorCode.x = code % 255;
    code = code / 255;

    return colorCode;
}

glm::vec4 TranslateToNormalizedColorCode(unsigned int code) {
    auto color = TranslateToColorCode(code);
    return glm::vec4(color.x / 255.0, color.y / 255.0, color.z / 255.0, color.w / 255.0);
}
