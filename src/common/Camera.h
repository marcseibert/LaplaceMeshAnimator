#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SceneNode.h"
#include "CommonStructs.h"
#include "DraggableObject.h"

using namespace glm;

class Camera : public SceneNode, public DraggableObject {
public:
    Camera() {};
    Camera(int screenWidth, int screenHeight, float x, float y, float z, bool orthographic=false);
    
    mat4 GetCameraMatrix();
    void ApplyViewportChange(GLFWwindow* window, int width, int height);
    
    //void SetRotation(float pitch, float yaw);
    void SetViewport(float x, float y, float width, float height);

    void Update(GLFWwindow *window, float deltaTime) override;

    void ApplyDrag() override;

    Rect GetViewport() {
        return mViewport;
    };
    glm::vec3 GetViewDir();
    glm::vec3 ScreenToWorldSpace(glm::vec3 coords);
    glm::vec3 ScreenPointToRay(glm::vec2 point);
    void Drag(glm::vec3 dragVector) override;

    //void (glm::vec3 coords);

private:
    bool wasPressedP = false;
    void UpdateCameraMatrix();
    void SetupProjectionMatrix();

    mat4 mProjectionMatrix;
    mat4 mViewMatrix;

    mat4 mCameraMatrix;
    bool mOrthographic;
    Rect mViewport;
};
