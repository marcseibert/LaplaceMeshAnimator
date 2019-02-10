#include "Box.hpp"
#include <vector>
#include "../Util.hpp"
#include "../ShaderManager.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

Box::Box(Rect rect, glm::vec4 color)
: SceneNode(), RenderObject() {
    
    unsigned int indices[6] = {
        0, 1, 3, 3, 1, 2
    };
    
    offsetX = 0;
    offsetY = 0;
    offsetWidth = 1;
    offsetHeight = 1;

    bounds = rect;
    mColor = color;

    float newRectData[5 * 4] = {
        -1, -1, 0, offsetX, offsetY,
        -1,  1, 0, offsetX, offsetY + offsetHeight,
         1,  1, 0, offsetX + offsetWidth, offsetY + offsetHeight,
         1, -1, 0, offsetX + offsetWidth, offsetY
    };
    
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


    glBindVertexArray(0);
    
    UpdateModelMatrix();
};

Box::~Box() {
    
};

void Box::Update(GLFWwindow *window, float deltaTime) {

}

void Box::Draw(Camera &camera) {
    Shader *program = ShaderManager::getProgram(SM_SINGLE_COLOR);
    program->use();


    glUniform4fv(glGetUniformLocation(program->ID, "color"), 1, glm::value_ptr(glm::vec4(mColor, 1)));
    //std::cout << glGetUniformLocation(program->ID, "color") << std::endl;
    
    // CALCULATE MVP MATRIX
    auto mvp = camera.GetCameraMatrix() * modelMatrix;
    //auto mvp = modelMatrix;


    glUniformMatrix4fv(glGetUniformLocation(program->ID, "mvpMatrix"), 1, GL_FALSE, value_ptr(mvp));


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
};

void Box::UpdateModelMatrix() {
    modelMatrix = translate(mat4(1), glm::vec3(bounds.position.x, bounds.position.y, 0));
    modelMatrix = scale(modelMatrix, vec3(bounds.width, bounds.height, 1));
}
