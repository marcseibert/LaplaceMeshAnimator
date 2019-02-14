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

void EditableMesh::ClearSelection() {
    mHighlightedVertices.resize(0);

    ClearHandleColors();
    UpdateHandleColors();
}

// TODO: MAKE THIS MORE EFFICIENT...
void EditableMesh::ToggleVertexSelection(unsigned int vertexID) {

    SetVertexSelection(vertexID, !ContainsID(vertexID, mHighlightedVertices));
}

void EditableMesh::SetVertexSelection(unsigned int vertexID, bool b, glm::vec4 selectionColor) {
    if(!b) {
        mHighlightedVertices.remove(vertexID);
    } else {
        if(!ContainsID(vertexID, mHighlightedVertices)) {
            mHighlightedVertices.push_back(vertexID);
        }
    }

    mHandleColors[vertexID] = selectionColor;
    UpdateHandleColors();
}

void EditableMesh::SetVertexSelection(unsigned int vertexID, bool b) {
    SetVertexSelection(vertexID, b, b ? SELECTED_VERTEX_COLOR : UNSELECTED_VERTEX_COLOR);
}

void EditableMesh::SetSelectionSet(std::list<unsigned int> &selection, glm::vec4 selectionColor) {
    mHighlightedVertices = selection;

    SetSelectionSetColor(selectionColor);
    UpdateHandleColors();
}

void EditableMesh::SelectAll() {
    mHighlightedVertices.resize(0);
    for(int i = 0; i < mMesh->mVertices.size(); i++) {
        mHighlightedVertices.push_back(i);
    }
    SetSelectionSetColor(SELECTED_VERTEX_COLOR);
    UpdateHandleColors();
}

GLuint EditableMesh::CheckVertexIntersection(Renderer &renderer, Camera &camera, glm::vec2 position) {
    renderer.ClearIds();
    renderer.ActivateViewport();

    DrawVertexHandleIds(camera);

    unsigned char res[4];
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glReadPixels(position.x, viewport[3] - position.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);

    if(res[3] != 0) {
        unsigned int clickedObject = uiTranslateColorCode(glm::vec4(res[0], res[1], res[2], res[3]));
        //std::cout << " CLICKED ID " << clickedObject << std::endl;

        return clickedObject + 1;
    } else {
        return 0;
    }
    return 0;
};

void EditableMesh::UnselectVertexSet(std::list<unsigned int> &vertexSet, glm::vec4 color) {
    for(std::list<unsigned int>::iterator it = vertexSet.begin(); it != vertexSet.end(); it++) {
        mHighlightedVertices.remove(*it);
        mHandleColors[*it] = color;
    }

    UpdateHandleColors();
}
