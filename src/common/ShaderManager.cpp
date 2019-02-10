#include "ShaderManager.hpp"
#include <string>

ShaderManager::ShaderManager() {
};

ShaderManager::~ShaderManager() {
    //for(int i = 0; i < SM_NUM_ITEMS; i++) {
    //    free(shaderMap[i]);
    //}
};

Shader ShaderManager::shaderMap[SM_NUM_ITEMS];

void ShaderManager::init() {
    std::string shaderDirectory = "/Users/marcseibert/Documents/Development/git/LaplaceMeshAnimator/src/shader/";

    auto &spriteShader = shaderMap[SM_SPRITE];
    
    spriteShader.init();
    spriteShader.addShader(shaderDirectory + "DrawableShader.vert", GL_VERTEX_SHADER);
    spriteShader.addShader(shaderDirectory + "DrawableShader.frag", GL_FRAGMENT_SHADER);
    spriteShader.link();

    auto &singleColorShader = shaderMap[SM_SINGLE_COLOR];
    singleColorShader.init();
    singleColorShader.addShader(shaderDirectory + "SingleColorShader.vert", GL_VERTEX_SHADER);
    singleColorShader.addShader(shaderDirectory + "SingleColorShader.frag", GL_FRAGMENT_SHADER);
    singleColorShader.link();

    auto &meshShader = shaderMap[SM_COMPLEX_MESH];
    meshShader.init();
    meshShader.addShader(shaderDirectory + "ComplexMeshShader.vert", GL_VERTEX_SHADER);
    meshShader.addShader(shaderDirectory + "ComplexMeshShader.frag", GL_FRAGMENT_SHADER);
    meshShader.link();

}

GLuint ShaderManager::getPrgID(ShaderType type) { return shaderMap[type].ID; };
void ShaderManager::useProgram(ShaderType type) { shaderMap[type].use(); };
Shader* ShaderManager::getProgram(ShaderType type) { return &shaderMap[type]; };
