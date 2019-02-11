//
// Created by Marc Seibert on 2019-02-09.
//

#ifndef LAPLACEMESHANIMATOR_INSPECTORVIEW_H
#define LAPLACEMESHANIMATOR_INSPECTORVIEW_H

#include "common/View.h"
#include "common/drawables/Box.h"
#include "common/Renderer.h"
#include "common/input/MouseInput.h"

class InspectorView : public View {
public:
    InspectorView() : View() { };

    InspectorView(float viewX, float viewY, float viewWidth, float viewHeight, MouseInput* mouseInput)
    : View(viewX, viewY, viewWidth, viewHeight, mouseInput) {

        SetClearColor(glm::vec4(1,0,1,1));
        Repaint();


        // DEFINE UI OBJECTS HERE
        mainCamera = Camera(viewWidth, viewHeight,0,0, -5, true);
        testBox = Box(Rect(0, 0, 200.0f, 200.0f), glm::vec4(1,1,0,1));

        testBox.UpdateGlobalTransform();
    };


    void Update(GLFWwindow *window, float deltaTime) override {
        if(!mActive) {
            return;
        }

        mainCamera.Update(window, deltaTime);
    };

    void Draw() override {
        SetViewport();

        if(mActive) {

            glClearColor(1.0, 1.0, 0.95, mClearColor.a);
        } else {
            glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.Draw(mBounds, mainCamera, testBox);
    };

    void UpdateWindowParameters() override {
        //mainCamera.SetViewport(mBounds.position.x, mBounds.position.x, mBounds.width, mBounds.height);
    };


private:
    Box testBox;
    Renderer renderer;
    Camera mainCamera;
};
#endif //LAPLACEMESHANIMATOR_INSPECTORVIEW_H
