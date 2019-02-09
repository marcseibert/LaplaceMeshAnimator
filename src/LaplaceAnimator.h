//
// Created by Marc Seibert on 2019-02-08.
//

#ifndef LAPLACEMESHANIMATOR_LAPLACEANIMATOR_H
#define LAPLACEMESHANIMATOR_LAPLACEANIMATOR_H

#include "common/GraphicsProgram.h"
#include "common/View.h"
#include "InspectorView.h"

#include <iostream>

class LaplaceAnimator : public GraphicsProgram {
public:
    LaplaceAnimator(GLFWwindow* window) : GraphicsProgram(window) { }

    void Initialise(int screenWidth, int screenHeight) override {

        sceneView = View(0, 0, screenWidth / 2, screenHeight);
        inspector= InspectorView(screenWidth / 2,0, screenWidth / 2, screenHeight);

        std::cout << " " << screenWidth / 2 << std::endl;


        sceneView.SetClearColor(glm::vec4(1,0,0,1));
        sceneView.Repaint();

        glClearColor(0,0,0,1);
    };

    void Update() override {
        sceneView.Update();

        inspector.Update();
    };

    void Draw() override {
        glClear(GL_COLOR_BUFFER_BIT);
        // ONLY REDRAW IF NEEDED
        if(sceneView.IsDirty()) {
            sceneView.Draw();
        }

        if(inspector.IsDirty()) {
            inspector.Draw();
        }
    };

    ~LaplaceAnimator() override {

    };

private:
    View sceneView;
    InspectorView inspector;
};


#endif //LAPLACEMESHANIMATOR_LAPLACEANIMATOR_H
