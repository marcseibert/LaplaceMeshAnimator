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

class SceneView : public View {
public:
    SceneView() : View() { };

    SceneView(GLFWwindow *window, float viewX, float viewY, float viewWidth, float viewHeight, MouseInput* mouseInput)
    : View(viewX, viewY, viewWidth, viewHeight, mouseInput), renderer(window) {

        SetClearColor(glm::vec4(1,1,1,1));
        Repaint();
        // DEFINE UI OBJECTS HERE
        mainCamera = Camera(viewWidth, viewHeight,0,0, -10, false);
        //testModel = new Model("/Users/marcseibert/Downloads/nanosuit/nanosuit.obj", 0, 0, 0);
        //testModel = new Model("/Users/marcseibert/Desktop/pyramid.obj", 0, 0, 0);

        /*
        glm::vec3 positions[] = {
                glm::vec3(-0.4f,)
        };*/
        float range = 2;

        for(int i = 0; i < 15; i++) {
            testModels.push_back(new Model("/Users/marcseibert/Downloads/nanosuit/nanosuit.obj", randNumber(-10, 10), randNumber(-10, 10), randNumber(-10, 10)));
        }
        /*
        for(int i = 0; i < 15; i++) {
            testModels.push_back(new Model("/Users/marcseibert/Desktop/prim_cube.obj", randNumber(-10, 10), randNumber(-10, 10), randNumber(-10, 10)));
        }
        */

        for(auto &model : testModels) {
            editWrappers.push_back(EditableModel(*model, editWrappers.size()));
        }
        unsigned int test = 25345;
        glm::vec4 colorCode = TranslateToColorCode(test);
        unsigned int output = uiTranslateColorCode(colorCode);

        std::cout << " TEST " << test << " = " << output << std::endl;

        //testModel->SetScale(0.2f, 0.2f, 0.2f);
        //testBox = Box(Rect(0, 0, 100.0f, 100.0f), glm::vec4(1,1,0,1));

        //testBox.UpdateGlobalTransform();
    };


    void Update(GLFWwindow *window, float deltaTime) override {
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            testModels[0]->Translate(0.01f,0,0);
        }

        if(!mActive) {
            return;
        }

        if(mouseInput) {
            auto mousePosition = mouseInput->GetPosition();
            if(mouseInput->IsDragged()) {
                //
                glm::vec3 worldSpaceDrag = mainCamera.ScreenToWorldSpace(glm::vec3(mouseInput->GetDrag(), 0));
                //std::cout << "drag-x: " << worldSpaceDrag.x << " drag-y: " << worldSpaceDrag.y << std::endl;
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
                    renderer.DrawIdObject(mBounds, mainCamera, wrapper);
                }

                unsigned char res[4];
                GLint viewport[4];
                glGetIntegerv(GL_VIEWPORT, viewport);
                glReadPixels(mousePosition.x, viewport[3] - mousePosition.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);
                ClearSelection();

                if(res[3] != 0) {
                    unsigned int clickedObject = uiTranslateColorCode(glm::vec4(res[0], res[1], res[2], res[3]));
                    std::cout << " CLICKED ID " << clickedObject << std::endl;
                    editWrappers[clickedObject].SetSelected(true);
                }

            }
        }

        //mainCamera.ScreenPointToRay(glm::vec2(mouseInput->GetPosition()));

        mainCamera.Update(window, deltaTime);
    };

    void Draw() override {
        SetViewport();

        if(mActive) {

            glClearColor(1.0, 1.0, 0.8, mClearColor.a);
        } else {

            glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /*
        for(Model* model : testModels) {
            renderer.Draw(mBounds, mainCamera, *model);
        } */


        for(auto &wrapper : editWrappers) {
            renderer.Draw(mBounds, mainCamera, wrapper);
        }

        //renderer.Draw(mBounds, mainCamera, testBox);
        //renderer.Draw(mBounds, mainCamera, *testModel);


    };

    void UpdateWindowParameters() override {
        //mainCamera.SetViewport(mBounds.position.x, mBounds.position.x, mBounds.width, mBounds.height);
        std::cout << " SOMETHING CHANGES " << std::endl;

    };

    ~SceneView() override {
        /*
        for(int i = 0; i < 15; i++) {
            delete testModels[i];
        }*/
    };

private:
    //Box testBox;
    std::vector<Model*> testModels;
    std::vector<EditableModel> editWrappers;
    Renderer renderer;
    Camera mainCamera;

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
