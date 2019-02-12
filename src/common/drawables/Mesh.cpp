#include "Mesh.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<glm::uvec3> &faces, std::vector<Texture> &textures)
: SceneNode(0, 0, 0), RenderObject() {
    this->mVertices = vertices;
    this->mFaces = faces;
    this->mTextures = textures;

    setupMesh();
}

void Mesh::setupMesh() {

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mFaces.size() * sizeof(glm::uvec3), &mFaces[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void Mesh::Update(GLFWwindow *window, float deltaTime) {};

void Mesh::UpdateModel() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);
}

void Mesh::Draw(Camera &camera) {

    auto *shader = ShaderManager::getProgram(SM_COMPLEX_MESH);

    shader->use();

    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for(unsigned int i = 0; i < mTextures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = mTextures[i].type;
        if(name == "texture_diffuse"){
            //cout << " DRAWING TEXTURE " << diffuseNr << std::endl;
            number = std::to_string(diffuseNr++);
        }
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);

        glUniform1i(glGetUniformLocation(shader->ID, ("material." + name + number).c_str()), i);
        //std::cout << ("material." + name + number).c_str() << " " << glGetUniformLocation(shader->ID, ("material." + name + number).c_str()) << " TEX POS" << std::endl;
        glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
    }

    DrawCall(camera, *shader);
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::DrawCall(Camera &camera, Shader &shader) {
    auto mvp = camera.GetCameraMatrix() * globalTransform; // localTransform;//glm::scale(localTransform, glm::vec3(10,10,10));//globalTransform;
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "mvpMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, mFaces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

std::vector<Vertex>* Mesh::GetVertices() {
    return &mVertices;
}

std::vector<glm::uvec3>* Mesh::GetFaces() {
    return &mFaces;
}

bool Mesh::RayIntersects(glm::vec4 ray) {
    return false;
}

vec3 Mesh::RayIntersectsAt(glm::vec4 ray) {
    return glm::vec3(1);
}