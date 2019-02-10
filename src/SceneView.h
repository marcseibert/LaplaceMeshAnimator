//
// Created by Marc Seibert on 2019-02-10.
//

#ifndef LAPLACEMESHANIMATOR_SCENEVIEW_H
#define LAPLACEMESHANIMATOR_SCENEVIEW_H

#include "common/View.h"
#include "common/Renderer.h"
#include "common/input/MouseInput.h"
#include "common/drawables/Model.hpp"
#include "common/drawables/Box.hpp"

class SceneView : public View {
public:
    SceneView() : View() { };

    SceneView(float viewX, float viewY, float viewWidth, float viewHeight, MouseInput* mouseInput)
    : View(viewX, viewY, viewWidth, viewHeight, mouseInput) {

        SetClearColor(glm::vec4(1,1,1,1));
        Repaint();


        // DEFINE UI OBJECTS HERE
        mainCamera = Camera(viewWidth, viewHeight,0,0, -10, true);
        testModel = new Model("/Users/marcseibert/Downloads/nanosuit/nanosuit.obj", 0, 0, 0);
        //testModel = new Model("/Users/marcseibert/Desktop/pyramid.obj", 0, 0, 0);

        testModel->SetScale(50, 50, 50);
        //testBox = Box(Rect(0, 0, 100.0f, 100.0f), glm::vec4(1,1,0,1));

        //testBox.UpdateGlobalTransform();
    };


    void Update(GLFWwindow *window, float deltaTime) override {
        if(mouseInput) {
            if(mouseInput->IsDragged()) {
                mainCamera.Drag(glm::vec3(mouseInput->GetDrag(), 0));
                mainCamera.UpdateGlobalTransform();
            }
            if(!mouseInput->IsPressed(MOUSE_BUTTON_CENTER) && mainCamera.IsDragged()) {
                mainCamera.ApplyDrag();
                std::cout << " test" << std::endl;
            }
        }

        mainCamera.Update(window, deltaTime);
    };

    void Draw() override {
        SetViewport();

        glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //renderer.Draw(mBounds, mainCamera, testBox);
        renderer.Draw(mBounds, mainCamera, *testModel);


    };

    void UpdateWindowParameters() override {
        //mainCamera.SetViewport(mBounds.position.x, mBounds.position.x, mBounds.width, mBounds.height);
        std::cout << " SOMETHING CHANGES " << std::endl;

    };
private:
    //Box testBox;
    Model *testModel;
    Renderer renderer;
    Camera mainCamera;
};
#endif //LAPLACEMESHANIMATOR_SCENEVIEW_H
