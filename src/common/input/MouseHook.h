//
// Created by Marc Seibert on 2019-02-11.
//

#ifndef LAPLACEMESHANIMATOR_MOUSEHOOK_H
#define LAPLACEMESHANIMATOR_MOUSEHOOK_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../SceneNode.h"
#include "../RenderObject.h"
#include "../Camera.h"
#include "../modifier/EditableMesh.h"
#include "../CommonStructs.h"

class MouseHook : public SceneNode, public RenderObject {
public:

    MouseHook() : SceneNode(), RenderObject() {

    };

    void Update(GLFWwindow *window, float deltaTime) override {

    };

    void Draw(Camera &camera) override {

    };

private:
    Vertex *selectedVertex;
    EditableMesh *selectedMesh;
};
#endif //LAPLACEMESHANIMATOR_MOUSEHOOK_H
