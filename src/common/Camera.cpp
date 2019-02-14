#include "Camera.h"

//#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(int screenWidth, int screenHeight, float x, float y, float z, bool orthographic)
: SceneNode(x, y, z), DraggableObject(), mOrthographic(orthographic)  {

    SetViewport(x, y, screenWidth, screenHeight);
    std::cout << glm::to_string( mTransform.position) << std::endl;
    //UpdateCameraMatrix();
}

void Camera::ApplyViewportChange(GLFWwindow* window, int width, int height) {
    
}

void Camera::SetViewport(float x, float y, float width, float height) {
    mViewport = Rect(x, y, width, height);

    SetupProjectionMatrix();
    UpdateCameraMatrix();
}
void Camera::Update(GLFWwindow *window, float deltaTime) {

    const float speed = 10.0f;

    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        Translate(-speed * deltaTime,0,0);
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        Translate(speed * deltaTime,0,0);
    }

    if(glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
        Translate(0,0,speed * deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
        Translate(0,0, -speed * deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        Translate(0, -speed *deltaTime,0);
    }

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        Translate(0, speed * deltaTime, 0);
    }
}

mat4 Camera::GetCameraMatrix() {
 
    return mCameraMatrix;
}


void Camera::UpdateCameraMatrix() {
    UpdateLocalTransform();
    UpdateGlobalTransform();
    glm::vec4 globalPosition = globalTransform * glm::vec4(mTransform.position, 1); // + glm::vec4(mDragVector, 0);

    mViewMatrix = glm::lookAt(glm::vec3(globalPosition), glm::vec3(globalPosition) + glm::vec3(0, 0, 1), glm::vec3(0,1,0));
    mCameraMatrix = mProjectionMatrix * mViewMatrix;//* globalTransform;
}

void Camera::SetupProjectionMatrix() {

    if(mOrthographic) {
        // USE NORMALISED WORLD COORDINATES
        float ratio = ((float) mViewport.height / mViewport.width)*mTransform.scale.y;

        mProjectionMatrix = ortho(-mTransform.scale.x, mTransform.scale.x, -ratio, ratio, 0.1f, 1000.0f);
    } else{
        mProjectionMatrix = perspective(radians(60.0f), (float)mViewport.width / mViewport.height, 0.1f, 10000.0f);
    }
}

void Camera::ApplyDrag() {
    mDragVector = glm::vec3(0);
    mDeltaDrag = glm::vec3(0);

    mIsDragged = false;

    UpdateCameraMatrix();
}


glm::vec3 Camera::ScreenToWorldSpace(glm::vec3 coords) {
    float aspectRatio = mViewport.height / mViewport.width;

    // SCREENS SPACE - 0,0 IN TOP LEFT CORNER
    return glm::vec3((coords.x / mViewport.width)*2 *mTransform.scale.x, (coords.y/ mViewport.height) * 2 * aspectRatio * mTransform.scale.y, coords.z);
}

glm::vec3 Camera::ScreenPointToRay(glm::vec2 point) {
    float x = (2.0f * point.x) / mViewport.width - 1.0f;
    float y = 1.0f - (2.0f * point.y) / mViewport.height;

    glm::vec4 ray = glm::vec4(x, y, -1.0f, 1.0f);

    glm::vec4 rayEye = glm::inverse(mProjectionMatrix) * ray;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0);

    glm::vec3 rayWorld = glm::vec3(inverse(mViewMatrix) * rayEye);

    rayWorld = glm::normalize(rayWorld);

    std::cout << glm::to_string(rayWorld) << std::endl;
    return rayWorld;
};

void Camera::Drag(glm::vec3 dragVector) {
    mDeltaDrag = -mDragVector + dragVector;
    mDragVector = dragVector;

    Translate(mDeltaDrag.x / mTransform.scale.x, mDeltaDrag.y / mTransform.scale.y, mDeltaDrag.z / mTransform.scale.z);
    mIsDragged = true;
}

glm::vec3 Camera::GetViewDir() {
    return glm::vec3(0,0,1);
}
