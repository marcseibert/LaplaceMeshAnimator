#include "Sprite.h"
#include <vector>
#include "../Util.h"
#include "../ShaderManager.h"

Sprite::Sprite(string const &path, float x, float y, float z, float width, float height, Rect viewport)
: SceneNode(), RenderObject() {
    
    Shader *program = ShaderManager::getProgram(SM_SPRITE);
    unsigned int indices[6] = {
        0, 1, 3, 3, 1, 2
    };
    
    offsetX = 0;
    offsetY = 0;
    offsetWidth = 1;
    offsetHeight = 1;

    mBounds.position.x = x;
    mBounds.position.y = y;
    mBounds.width = width;
    mBounds.height = height;
    
    float newRectData[5 * 4] = {
        -1, -1, 0, offsetX + offsetWidth, offsetY,
        -1,  1, 0, offsetX + offsetWidth, offsetY + offsetHeight,
         1,  1, 0, offsetX, offsetY + offsetHeight,
         1, -1, 0, offsetX, offsetY
    };

    /*
    float newRectData[5 * 4] = {
            -1, -1, 0, offsetX, offsetY,
            -1,  1, 0, offsetX, offsetY + offsetHeight,
            1,  1, 0, offsetX + offsetWidth, offsetY + offsetHeight,
            1, -1, 0, offsetX + offsetWidth, offsetY
    };
     */

    // generate Rect
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), newRectData, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // LOAD TEXTURE
    textureID = TextureFromFile(path);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    std::cout << "TEST" <<glGetUniformLocation(program->ID, "tex") << std::endl;
    
    glBindVertexArray(0);
    
    UpdateModelMatrix(viewport);

    UpdateLocalTransform();
    UpdateGlobalTransform();
};

void Sprite::Update(GLFWwindow *window, float deltaTime) {

}

Sprite::~Sprite() {
    
};

void Sprite::Draw(Camera &camera) {
    Shader *program = ShaderManager::getProgram(SM_SPRITE);
    program->use();


    glUniform1i(glGetUniformLocation(program->ID, "tex"), 0);
    
    // CALCULATE MVP MATRIX
    auto mvp = camera.GetCameraMatrix() * globalTransform * mModelMatrix;
    
    glUniformMatrix4fv(glGetUniformLocation(program->ID, "mvpMatrix"), 1, GL_FALSE, value_ptr(mvp));

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
};

void Sprite::UpdateModelMatrix(Rect viewport) {
    float bRatio = mBounds.height / mBounds.width;
    float relativeWidth = mBounds.width / viewport.width;

    mModelMatrix = translate(mat4(1), glm::vec3(mBounds.position.x, mBounds.position.y, 0));
    mModelMatrix = scale(mModelMatrix, vec3(relativeWidth, relativeWidth * bRatio, 1));
}
