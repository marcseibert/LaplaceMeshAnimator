#include "EditableMesh.h"

#include <iostream>

EditableMesh::EditableMesh(Mesh &mesh, unsigned int id)
: drawMode(MESH_NORMAL_MODE), RenderIDObject(id), mSelected(false), mActive(false), mColor(NORMAL_MESH_COLOR) {
    mMesh = &mesh;
    UpdateColor();
}

void EditableMesh::Draw(Camera &camera) {
    auto &shader = *ShaderManager::getProgram(SM_SINGLE_COLOR);
    shader.use();

    glUniform4fv(glGetUniformLocation(shader.ID, "color"), 1, glm::value_ptr(mColor));
    mMesh->DrawCall(camera, shader);
}

void EditableMesh::DrawID(Camera &camera) {
    DrawID(camera, ID);
}

void EditableMesh::DrawID(Camera &camera, unsigned int id) {
    //std::cout << "DRAW MESH WITH ID " << ID << std::endl;
    auto &shader = *ShaderManager::getProgram(SM_CODE);
    shader.use();

    glUniform4iv(glGetUniformLocation(shader.ID, "code"), 1, glm::value_ptr(TranslateToColorCode(id)));
    mMesh->DrawCall(camera, shader);
}

void EditableMesh::ClearHighlights() {

}

void EditableMesh::HighlightVertex(unsigned int vertexID) {

}
