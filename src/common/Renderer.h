//
// Created by Marc Seibert on 2019-02-09.
//

#ifndef LAPLACEMESHANIMATOR_RENDERER_H
#define LAPLACEMESHANIMATOR_RENDERER_H
#include "RenderObject.h"
#include "CommonStructs.h"
#include "RenderIDObject.h"

#include  <GLFW/glfw3.h>

class Renderer {
public:
    Renderer() {};
    Renderer(GLFWwindow* window) : window(window) {};

    void Draw(Rect viewport, Camera &camera, RenderObject &object) {
        
        camera.SetViewport(viewport.position.x, viewport.position.y, viewport.width, viewport.height);
        object.Draw(camera);
    };

    void ClearIds() {
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        glViewport(0,0, width, height);
        glScissor(0,0, width, height);

        glClearColor(0,0,0,0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    };

    void DrawIdObject(Rect viewport, Camera &camera, RenderIDObject &object) {
        camera.SetViewport(viewport.position.x, viewport.position.y, viewport.width, viewport.height);
        object.DrawID(camera);
    };

private:
    GLFWwindow *window;
};
#endif //LAPLACEMESHANIMATOR_RENDERER_H
