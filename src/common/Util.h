#pragma once
#include <GL/glew.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>

GLchar* loadShaderFromFile(const char* path);
unsigned int TextureFromFile(std::string const &filename, bool gamma = false);

unsigned int uiTranslateColorCode(glm::vec4 colorCode);
glm::ivec4 TranslateToColorCode(unsigned int code);
glm::vec4 TranslateToNormalizedColorCode(unsigned int code);
