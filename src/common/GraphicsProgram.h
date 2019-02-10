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

    virtual void Initialise(int screenWidth, int screenHeight) = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;

    virtual void OnMouseMove(GLFWwindow* window, double xPos, double yPos) = 0;
    virtual void OnWindowRescale(GLFWwindow* window, int screenWidth, int screenHeight) = 0;

    virtual ~GraphicsProgram() = default;
protected:
    GLFWwindow *window;
};
#endif //LAPLACEMESHANIMATOR_GRAPHICSPROGRAM_H
