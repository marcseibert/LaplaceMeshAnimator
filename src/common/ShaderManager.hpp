#pragma once
#include <GL/glew.h>

#include <stdio.h>
#include <vector>
#include <map>
#include <array>
#include <string>
#include "Shader.hpp"

enum ShaderType {
    SM_SPRITE,
    SM_SINGLE_COLOR,
    SM_COMPLEX_MESH,
    SM_NUM_ITEMS
};

class ShaderManager {
public:
    ShaderManager();
    ~ShaderManager();
    
    static void init();
    static GLuint getPrgID(ShaderType type);
    static void useProgram(ShaderType type);
    static Shader* getProgram(ShaderType type);
    
private:
    static Shader shaderMap[SM_NUM_ITEMS];
};
