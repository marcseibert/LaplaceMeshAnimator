//
// Created by Marc Seibert on 2019-02-08.
//

#ifndef LAPLACEMESHANIMATOR_GRAPHICSPROGRAM_H
#define LAPLACEMESHANIMATOR_GRAPHICSPROGRAM_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GraphicsProgram {
public:
    GraphicsProgram();
    GraphicsProgram(GLFWwindow *window) { this->window = window; };

    virtual void Initialise() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;

protected:
    GLFWwindow *window;
};
#endif //LAPLACEMESHANIMATOR_GRAPHICSPROGRAM_H
