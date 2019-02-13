//
// Created by Marc Seibert on 2019-02-11.
//

#ifndef LAPLACEMESHANIMATOR_EDITABLEMODEL_H
#define LAPLACEMESHANIMATOR_EDITABLEMODEL_H
#include "../drawables/Model.h"
#include "EditableMesh.h"
#include <string>

class EditableModel : public RenderIDObject, public RenderObject {
public:
    EditableModel() = default;
    EditableModel(Model &model, unsigned int id = 0) : RenderIDObject(id), mSelected(false), selectedMesh(nullptr), isEditing(false) {
        mModel = &model;
        auto &meshes = *mModel->GetMeshes();

        for(auto &mesh : meshes){//auto iter = meshes.begin(); iter != meshes.end(); iter++){//auto &mesh : meshes) {
            mMeshes.push_back(EditableMesh(mesh, mMeshes.size()));
        }
    };

    void Draw(Camera &camera) override {
        for(auto &mesh : mMeshes) {
            mesh.Draw(camera);
        }
        if(mSelected) {
            glClear(GL_DEPTH_BUFFER_BIT);
            for(auto &mesh : mMeshes) {
                mesh.DrawWireframe(camera);
                mesh.DrawVertexHandles(camera);
                //mesh.DrawVertexHandleIds(camera);
            }
        }
    };

    void DrawID(Camera &camera) override {
        for(auto &mesh : mMeshes) {
            mesh.DrawID(camera, ID);
        }
    };

    GLuint CheckMeshIntersection(Renderer &renderer, Camera &camera, glm::vec2 position) {
        renderer.ClearIds();
        renderer.ActivateViewport();

        for(unsigned int meshID = 0; meshID < mMeshes.size(); meshID++) {
            mMeshes[meshID].DrawID(camera, meshID);
        }

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
    };

    GLuint CheckVertexIntersection(Renderer &renderer, Camera &camera, glm::vec2 position) {
        renderer.ClearIds();
        renderer.ActivateViewport();

        selectedMesh->DrawVertexHandleIds(camera);

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

    void Update(GLFWwindow *window, Renderer &renderer, Camera &camera, MouseInput &mouse, float deltaTime) {
        if(mouse.IsNewPressed(MOUSE_BUTTON_LEFT) && mSelected) {
            //selectedMesh->
            unsigned int clickedVertex = CheckVertexIntersection(renderer, camera, mouse.GetPosition());

            if(clickedVertex != 0) {
                clickedVertex--;
                std::cout << " CLICKED ON VERTEX " << clickedVertex << std::endl;
                selectedMesh->ToggleVertexSelection(clickedVertex);
            }
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

            if(!wasPressedA) {
                if(selectedMesh) {
                    if(selectedMesh->GetSelectedVertices()->size() > 0){
                        selectedMesh->ClearSelections();
                    } else {
                        selectedMesh->SelectAll();
                        std::cout << " SELECT ALL " << std::endl;
                    }
                }
            }

            wasPressedA = true;
        } else {
            wasPressedA = false;
        }
    };

    void SetSelected(bool b) {
        mSelected = b;

        for(auto &mesh : mMeshes) {
            mesh.SetActive(b);
        }
    }

    void SelectMesh(Renderer &renderer, Camera &camera, glm::vec2 mousePosition) {
        unsigned int selectedMeshID = CheckMeshIntersection(renderer, camera, mousePosition);
        if(selectedMeshID != 0) {
            selectedMeshID--;
            std::cout << " SELECT MESH " << selectedMeshID << std::endl;
            if(selectedMesh) {
                selectedMesh->SetSelected(false);
            }

            selectedMesh = &mMeshes[selectedMeshID];
            selectedMesh->SetSelected(true);
        }
    };

    EditableMesh* GetSelectedMesh() {
        return selectedMesh;
    }

protected:
    bool wasPressedA;
    Model *mModel;
    bool mSelected;
    bool isEditing;

    EditableMesh *selectedMesh;

    std::vector<EditableMesh> mMeshes;
};

#endif //LAPLACEMESHANIMATOR_EDITABLEMODEL_H
