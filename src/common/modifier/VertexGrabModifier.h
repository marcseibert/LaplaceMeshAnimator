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
    : MeshModifier(), mGrabActive(false), mDragVector(0), wasPressedG(false){ };

    void Update(GLFWwindow* window, Renderer &renderer, Camera &camera, MouseInput &mouse) {
        if(!mMesh || !mEnabled) {
            return;
        }

        if(mouse.IsNewPressed(MOUSE_BUTTON_LEFT) && mMesh->IsSelected()) {
            unsigned int clickedVertex = mMesh->CheckVertexIntersection(renderer, camera, mouse.GetPosition());

            if(clickedVertex != 0) {
                clickedVertex--;
                std::cout << " CLICKED ON VERTEX " << clickedVertex << std::endl;
                mMesh->ToggleVertexSelection(clickedVertex);
            }
        }

        if(glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {

            if(!wasPressedG) {
                mGrabActive = !mGrabActive;
                if(mGrabActive) {
                    mGrabOrigin = glm::vec3(mouse.GetPosition(),0);
                } else {
                    mDragVector = glm::vec3(0);
                }
            }

            wasPressedG = true;
        } else {
            wasPressedG = false;
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

    void Enable() override {
        mEnabled = true;

        mGrabActive = false;
        mDragVector = glm::vec3(0);
    };

    void Disable() override {
        mEnabled = false;

        mGrabActive = false;
        mDragVector = glm::vec3(0);
    };

private:
    bool wasPressedG;
    bool mGrabActive;

    glm::vec3 mDragVector;
    glm::vec3 mDeltaDrag;
    glm::vec3 mGrabOrigin;

};
#endif //LAPLACEMESHANIMATOR_VERTEXGRABMODIFIER_H
