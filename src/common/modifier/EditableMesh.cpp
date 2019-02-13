#include "EditableMesh.h"

#include <iostream>

EditableMesh::EditableMesh(Mesh &mesh, unsigned int id)
: drawMode(MESH_NORMAL_MODE), RenderIDObject(id), mSelected(false), mActive(false), mColor(NORMAL_MESH_COLOR), mHandleSize(10){

    mMesh = &mesh;

    // GENERATE VERTEX ID COLORS
    glGenBuffers(1, &mColorCodeBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mColorCodeBuffer);
    unsigned int count = mesh.mVertices.size();
    for(unsigned int i =0; i < count; i++) {
        mColorCodes.push_back(TranslateToNormalizedColorCode(i));
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * mColorCodes.size(), &mColorCodes[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*) 0);

    UpdateColor();

    mHandleColors.resize(mMesh->mVertices.size());
    glGenBuffers(1, &mHandleColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mHandleColorBuffer);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*) 0);

    UpdateHandleColors();
}

void EditableMesh::Draw(Camera &camera) {
    auto &shader = *ShaderManager::getProgram(SM_SINGLE_COLOR);
    shader.use();

    glUniform4fv(glGetUniformLocation(shader.ID, "color"), 1, glm::value_ptr(mColor));
    glUniform3fv(glGetUniformLocation(shader.ID, "viewDir"), 1, glm::value_ptr(camera.GetViewDir()));
    glUniform1f(glGetUniformLocation(shader.ID, "diffuseStrength"), 0.1f);
    mMesh->DrawCall(camera, shader);

}

void EditableMesh::DrawWireframe(Camera &camera) {
    auto&shader = *ShaderManager::getProgram(SM_SINGLE_COLOR);
    shader.use();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_POINT_SMOOTH);
    glUniform4fv(glGetUniformLocation(shader.ID, "color"), 1, glm::value_ptr(glm::vec4(0)));

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    mMesh->DrawCall(camera, shader);
}

void EditableMesh::DrawVertexHandles(Camera &camera) {
    auto &shader = *ShaderManager::getProgram(SM_MULTI_COLOR);
    shader.use();

    glPointSize(mHandleSize);
    glDisable(GL_POINT_SMOOTH);
    auto mvp = camera.GetCameraMatrix() * mMesh->GetGlobalTransform(); // localTransform;//glm::scale(localTransform, glm::vec3(10,10,10));//globalTransform;
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "mvpMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));

    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    // ENABLE VERTEX ATTRIB
    glBindVertexArray(mMesh->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mHandleColorBuffer);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*) 0);

    glDrawElements(GL_TRIANGLES, mMesh->mFaces.size() * 3, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ARRAY_BUFFER, mMesh->VBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void EditableMesh::DrawVertexHandleIds(Camera &camera) {
    auto &shader = *ShaderManager::getProgram(SM_MULTI_COLOR);
    shader.use();

    glPointSize(mHandleSize);
    glDisable(GL_POINT_SMOOTH);
    auto mvp = camera.GetCameraMatrix() * mMesh->GetGlobalTransform(); // localTransform;//glm::scale(localTransform, glm::vec3(10,10,10));//globalTransform;
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "mvpMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));

    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    // ENABLE VERTEX ATTRIB
    glBindVertexArray(mMesh->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mColorCodeBuffer);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*) 0);

    glDrawElements(GL_TRIANGLES, mMesh->mFaces.size() * 3, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ARRAY_BUFFER, mMesh->VBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

void EditableMesh::ClearSelections() {
    mHighlightedVertices.resize(0);
    UpdateHandleColors();
}

// TODO: MAKE THIS MORE EFFICIENT...
void EditableMesh::ToggleVertexSelection(unsigned int vertexID) {
    if(ContainsID(vertexID, mHighlightedVertices)) {
        mHighlightedVertices.remove(vertexID);
    } else {
        mHighlightedVertices.push_back(vertexID);
    }

    UpdateHandleColors();
}

void EditableMesh::SelectAll() {
    mHighlightedVertices.resize(0);
    for(int i = 0; i < mMesh->mVertices.size(); i++) {
        mHighlightedVertices.push_back(i);
    }

    UpdateHandleColors();
}

