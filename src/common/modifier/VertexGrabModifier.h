//
// Created by Marc Seibert on 2019-02-12.
//

#ifndef LAPLACEMESHANIMATOR_VERTEXGRABMODIFIER_H
#define LAPLACEMESHANIMATOR_VERTEXGRABMODIFIER_H
#include "MeshModifier.h"
#include <glm/glm.hpp>
#include "../input/MouseInput.h"

class VertexGrabModifier : public MeshModifier {
public:
    VertexGrabModifier()
    : MeshModifier(), mGrabActive(false), mDragVector(0), wasPressed(false){ };

    void Update(GLFWwindow* window, Camera &camera, MouseInput &mouse) {
        if(!mMesh) {
            return;
        }

        if(glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {

            if(!wasPressed) {
                mGrabActive = !mGrabActive;
                if(mGrabActive) {
                    mGrabOrigin = glm::vec3(mouse.GetPosition(),0);
                } else {
                    mDragVector = glm::vec3(0);
                }
            }

            wasPressed = true;
        } else {
            wasPressed = false;
        }

        if(!mGrabActive) {
            return;
        }

        glm::vec3 worldSpaceDrag = camera.ScreenToWorldSpace(mGrabOrigin - glm::vec3(mouse.GetPosition(), 0));
        std::cout << "drag-x: " << worldSpaceDrag.x << " drag-y: " << worldSpaceDrag.y << std::endl;

        // REVERT PREVIOUS CHANGES
        mDeltaDrag = -mDragVector + worldSpaceDrag;
        mDragVector = worldSpaceDrag;

        ComputeModifier();
    };

    void ComputeModifier() override {
        glm::mat4 inverse = glm::inverse(mMesh->mMesh->GetGlobalTransform());
        for(auto &vertexID : *mMesh->GetSelectedVertices()) {
            auto &vertex = mMesh->mMesh->mVertices[vertexID];
            vertex.Position += glm::vec3(inverse * glm::vec4(mDeltaDrag,0));
        }

        mMesh->mMesh->UpdateModel();
    };

    void BindMesh(EditableMesh *mesh) override {
        mMesh = mesh;
    };

private:
    bool wasPressed;
    bool mGrabActive;

    glm::vec3 mDragVector;
    glm::vec3 mDeltaDrag;
    glm::vec3 mGrabOrigin;

};
#endif //LAPLACEMESHANIMATOR_VERTEXGRABMODIFIER_H
