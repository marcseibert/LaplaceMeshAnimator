//
// Created by Marc Seibert on 2019-02-09.
//

#ifndef LAPLACEMESHANIMATOR_INSPECTORVIEW_H
#define LAPLACEMESHANIMATOR_INSPECTORVIEW_H

#include "common/View.h"
#include "common/drawables/Box.h"
#include "common/Renderer.h"
#include "common/input/MouseInput.h"
#include "common/drawables/Sprite.h"

class InspectorView : public View {
public:
    InspectorView() : View() { };

    InspectorView(GLFWwindow *window, float viewX, float viewY, float viewWidth, float viewHeight, MouseInput* mouseInput)
    : View(viewX, viewY, viewWidth, viewHeight, mouseInput), renderer(window, Rect(viewX, viewY, viewWidth, viewHeight)) {

        SetClearColor(glm::vec4(1,1,1,1));
        Repaint();


        // DEFINE UI OBJECTS HERE
        mainCamera = Camera(viewWidth, viewHeight,0,0, -5, true);
        orthoSprite = Sprite("../assets/ortho.png", 0, 4.0f, 1, 200, 75, Rect(viewX, viewY, viewWidth, viewHeight));
        perspSprite = Sprite("../assets/persp.png", 0, 4.0f, 0, 200, 75, Rect(viewX, viewY, viewWidth, viewHeight));

        orthoSprite.SetScale(0.8f,0.8f,0.8f);
        perspSprite.SetScale(0.8f,0.8f,0.8f);
        orthoSprite.UpdateGlobalTransform();
        perspSprite.UpdateGlobalTransform();
        //Box(Rect(0, 0, 200.0f, 200.0f), glm::vec4(1,1,0,1));

        //testBox.UpdateGlobalTransform();
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

        if(perspMode) {
            renderer.Draw(mainCamera, perspSprite);
        } else {
            renderer.Draw(mainCamera, orthoSprite);
        }

        //renderer.Draw(mainCamera, mSprite);
    };

    void UpdateWindowParameters() override {
        //mainCamera.SetViewport(mBounds.position.x, mBounds.position.x, mBounds.width, mBounds.height);
        renderer.SetViewport(mBounds);
    };

    void SetPerspectiveMode(bool b) {
        perspMode = b;
    };


private:
    bool perspMode = false;
    Sprite perspSprite, orthoSprite;
    Renderer renderer;
    Camera mainCamera;
};
#endif //LAPLACEMESHANIMATOR_INSPECTORVIEW_H
