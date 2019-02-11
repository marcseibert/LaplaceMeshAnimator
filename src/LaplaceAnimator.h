//
// Created by Marc Seibert on 2019-02-08.
//

#ifndef LAPLACEMESHANIMATOR_LAPLACEANIMATOR_H
#define LAPLACEMESHANIMATOR_LAPLACEANIMATOR_H

#include "common/GraphicsProgram.h"
#include "InspectorView.h"
#include "SceneView.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include "common/input/MouseInput.h"

class LaplaceAnimator : public GraphicsProgram {
public:
    LaplaceAnimator(GLFWwindow* window) : GraphicsProgram(window) { }

    void Initialise(int screenWidth, int screenHeight) override {

        int sceneWidth =  (screenWidth < inspectorSize) ? 0 : screenWidth - inspectorSize;
        mouse = MouseInput(screenWidth, screenHeight, Rect(), 5.0f);

        scene = SceneView(window, 0, 0, sceneWidth, screenHeight, &mouse);
        inspector= InspectorView(window, sceneWidth, 0 , inspectorSize, screenHeight, &mouse);

        glClearColor(0,0,0,1);
    };

    void Update() override {
        double currentTime = glfwGetTime();
        double deltaTime = abs(currentTime - oldTime);
        oldTime = currentTime;
        mouse.Update(window);

        mouse.SetViewport(scene.GetViewport());
        auto mousePosition = mouse.GetPosition();
        //std::cout << "mouse-x: " << mousePosition.x << " mouse-y: " << mousePosition.y << " pressed: " << mouse.IsPressed(MOUSE_BUTTON_LEFT) << " dragged: " << mouse.IsDragged() << std::endl;


        if(mouse.IsNewPressed(MOUSE_BUTTON_LEFT)) {

            std::cout << "CLICK" << std::endl;
            scene.SetActive(scene.InsideRect(glm::vec3(mousePosition, 0)));

            inspector.SetActive(inspector.InsideRect(glm::vec3(mousePosition, 0)));
        }
        scene.Update(window, deltaTime);

        inspector.Update(window, deltaTime);
    };

    void Draw() override {

        glClear(GL_COLOR_BUFFER_BIT);
        // ONLY REDRAW IF NEEDED
        if(scene.IsDirty()) {
            scene.Draw();
        }

        if(inspector.IsDirty()) {
            inspector.Draw();
        }
    };

    void OnWindowRescale(GLFWwindow *window, int screenWidth, int screenHeight) override {

        int sceneWidth =  (screenWidth < inspectorSize) ? 0 : screenWidth - inspectorSize;
        scene.SetBounds(0, 0, sceneWidth, screenHeight);
        scene.Repaint();
        inspector.SetBounds(sceneWidth , 0 , inspectorSize, screenHeight);
        inspector.Repaint();

        scene.UpdateWindowParameters();
        inspector.UpdateWindowParameters();
    };

    void OnMouseMove(GLFWwindow* window, double xPos, double yPos) override {
        mouse.OnMouseMove(window, xPos, yPos);

        //std::cout << xPos << " || " << yPos << std::endl;
    };

    ~LaplaceAnimator() override {

    };

private:
    SceneView scene;
    InspectorView inspector;
    MouseInput mouse;

    double oldTime;
    int inspectorSize = 200;
};


#endif //LAPLACEMESHANIMATOR_LAPLACEANIMATOR_H
