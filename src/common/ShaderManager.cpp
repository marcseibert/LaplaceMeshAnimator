#include "ShaderManager.hpp"

ShaderManager::ShaderManager() {
};

ShaderManager::~ShaderManager() {
    //for(int i = 0; i < SM_NUM_ITEMS; i++) {
    //    free(shaderMap[i]);
    //}
};

Shader ShaderManager::shaderMap[SM_NUM_ITEMS];

void ShaderManager::init() {
    auto &spriteShader = shaderMap[SM_SPRITE_SHADER];
    
    spriteShader.init();
    spriteShader.addShader("/Users/marcseibert/Documents/Development/Sandbox/cfx/Globe/Globe/shader/DrawableShader.vert", GL_VERTEX_SHADER);
    spriteShader.addShader("/Users/marcseibert/Documents/Development/Sandbox/cfx/Globe/Globe/shader/DrawableShader.frag", GL_FRAGMENT_SHADER);
    spriteShader.link();
    
    //shaderMap[SM_SPRITE_SHADER] = spriteShader;
    
    std::cout << "INITED " << shaderMap[SM_SPRITE_SHADER].ID << " " << std::endl;
}

GLuint ShaderManager::getPrgID(ShaderType type) { return shaderMap[type].ID; };
void ShaderManager::useProgram(ShaderType type) { shaderMap[type].use(); };
Shader* ShaderManager::getProgram(ShaderType type) { return &shaderMap[type]; };
