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
    Renderer(GLFWwindow* window, Rect viewport) : window(window), mViewport(viewport) {};

    void Draw(Camera &camera, RenderObject &object) {

        ActivateViewport();
        camera.SetViewport(mViewport.position.x, mViewport.position.y, mViewport.width, mViewport.height);
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

    void DrawIdObject(Camera &camera, RenderIDObject &object) {
        ActivateViewport();

        camera.SetViewport(mViewport.position.x, mViewport.position.y, mViewport.width, mViewport.height);
        object.DrawID(camera);
    };

    void ActivateViewport() {
        glScissor(mViewport.position.x, mViewport.position.y, mViewport.width, mViewport.height);
        glViewport(mViewport.position.x, mViewport.position.y, mViewport.width, mViewport.height);
    }

    void SetViewport(Rect viewport) {
        mViewport = viewport;
    }

private:
    GLFWwindow *window;
    Rect mViewport;
};
#endif //LAPLACEMESHANIMATOR_RENDERER_H
