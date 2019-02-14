//
// Created by Marc Seibert on 2019-02-10.
//

#ifndef LAPLACEMESHANIMATOR_SCENEVIEW_H
#define LAPLACEMESHANIMATOR_SCENEVIEW_H
#include "common/View.h"
#include "common/Renderer.h"
#include "common/input/MouseInput.h"
#include "common/drawables/Model.h"
#include "common/drawables/Box.h"
#include "common/modifier/EditableModel.h"
#include "common/Util.h"
#include "common/modifier/VertexGrabModifier.h"
#include "common/modifier/LaplaceMeshModifier.h"

class SceneView : public View {
public:
    SceneView() : View() { };

    SceneView(GLFWwindow *window, float viewX, float viewY, float viewWidth, float viewHeight, MouseInput* mouseInput)
    : View(viewX, viewY, viewWidth, viewHeight, mouseInput), renderer(window, Rect(viewX, viewY, viewWidth, viewHeight)), grabModifier() {
        SetClearColor(glm::vec4(1,1,1,1));
        Repaint();

        // DEFINE UI OBJECTS HERE
        mainCamera = Camera(viewWidth, viewHeight,0,0, -10, true);

        float range = 2;
/*
        for(int i = 0; i < 10; i++) {
            //
            Model *newModel = new Model("/Users/marcseibert/Desktop/susanne.obj", randNumber(-10, 10), randNumber(-10, 10), randNumber(-10, 10) + 20);
            newModel->SetScale(randNumber(0.2f, 0.4f), randNumber(0.2f, 0.4f), randNumber(0.2f, 0.4f));
            newModel->UpdateGlobalTransform();
            testModels.push_back(newModel);

        } */
        testModels.push_back(new Model("/Users/marcseibert/Desktop/susanne.obj", 0, 0, 0));
        testModels[0]->UpdateGlobalTransform();

        for(auto &model : testModels) {
            editWrappers.push_back(EditableModel(*model, editWrappers.size()));
        }
    };

    void SetZoom(float x, float y, float z) {
        mainCamera.SetScale(x, y, z);
    }

    void Update(GLFWwindow *window, float deltaTime) override {
        if(!mActive) {
            return;
        }

        if(mouseInput) {
            auto mousePosition = mouseInput->GetPosition();
            if(mouseInput->IsDragged()) {
                glm::vec3 worldSpaceDrag = mainCamera.ScreenToWorldSpace(glm::vec3(mouseInput->GetDrag(), 0));

                mainCamera.Drag(worldSpaceDrag);
                mainCamera.UpdateGlobalTransform();
            }
            if(!mouseInput->IsPressed(MOUSE_BUTTON_CENTER) && mainCamera.IsDragged()) {
                mainCamera.ApplyDrag();
                std::cout << " test" << std::endl;
            }
            // UPDATE OBJECTION SELECTION
            if(mouseInput->IsNewPressed(MOUSE_BUTTON_RIGHT)) {
                renderer.ClearIds();

                SetViewport();
                for(auto &wrapper : editWrappers) {
                    renderer.DrawIdObject(mainCamera, wrapper);
                }

                unsigned char res[4];
                GLint viewport[4];
                glGetIntegerv(GL_VIEWPORT, viewport);
                glReadPixels(mousePosition.x, viewport[3] - mousePosition.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);
                ClearSelection();

                if(res[3] != 0) {
                    unsigned int clickedObject = uiTranslateColorCode(glm::vec4(res[0], res[1], res[2], res[3]));
                    std::cout << " CLICKED ID " << clickedObject << std::endl;

                    // DIRECTLY SELECT A CORRESPONDING MESH INSIDE THE MODEL
                    editWrappers[clickedObject].SetSelected(true);
                    editWrappers[clickedObject].SelectMesh(renderer, mainCamera, mousePosition);
                    grabModifier.BindMesh(editWrappers[clickedObject].GetSelectedMesh());
                    laplaceMeshModifier.BindMesh(editWrappers[clickedObject].GetSelectedMesh());
                }

            }
        }

        grabModifier.Update(window, renderer, mainCamera, *mouseInput);
        laplaceMeshModifier.Update(window, renderer,mainCamera, *mouseInput, deltaTime);
        for(auto &wrapper :editWrappers) {
            wrapper.Update(window, renderer, mainCamera, *mouseInput, deltaTime);
        }

        mainCamera.Update(window, deltaTime);
    };

    void SetEditMode(int mode) {
        mEditMode = mode;

        if(mEditMode == 0) {
            laplaceMeshModifier.Disable();
            grabModifier.Enable();
        } else {
            laplaceMeshModifier.Enable();
            grabModifier.Disable();
        }
    }

    void Draw() override {
        SetViewport();
        if(mActive) {

            glClearColor(1.0, 1.0, 0.8, mClearColor.a);
        } else {

            glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(auto &wrapper : editWrappers) {
            renderer.Draw(mainCamera, wrapper);
        }
    };

    void SetPerspectiveMode(bool b) {
        mainCamera.SetPerspective(b);
    }

    void UpdateWindowParameters() override {
        renderer.SetViewport(mBounds);
    };

    ~SceneView() override { };

private:
    int mEditMode;
    //Box testBox;
    std::vector<Model*> testModels;
    std::vector<EditableModel> editWrappers;
    Renderer renderer;
    Camera mainCamera;

    // MODIFIER
    VertexGrabModifier grabModifier;
    LaplaceMeshModifier laplaceMeshModifier;

    // DEBUG METHOD
    float randNumber(float min, float max) {

        std::cout << (static_cast<float>( rand()) / (static_cast<float>(RAND_MAX) / (max - min))) + min << std::endl;
        return (static_cast<float>( rand()) / (static_cast<float>(RAND_MAX) / (max - min))) + min ;
    }

    // EDITOR SELECTION METHODS

    void ClearSelection() {
        for(auto &wrapper : editWrappers) {
            wrapper.SetSelected(false);
        }
    }
};
#endif //LAPLACEMESHANIMATOR_SCENEVIEW_H
