#pragma once
#include <GL/glew.h>
#include <stdio.h>
#include <string>
#include <fstream>

GLchar* loadShaderFromFile(const char* path);
unsigned int TextureFromFile(std::string const &filename, bool gamma = false);
