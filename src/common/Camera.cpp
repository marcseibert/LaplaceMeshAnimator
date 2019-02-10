#include "Camera.hpp"

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
    const float speed = 100.0f;

    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        Translate(speed * deltaTime,0,0);
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        Translate(-speed * deltaTime,0,0);
    }

    if(glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
        Translate(0,0,-speed * deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
        Translate(0,0,speed * deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        Translate(0,speed *deltaTime,0);
    }

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        Translate(0,-speed * deltaTime, 0);
    }
}

mat4 Camera::GetCameraMatrix() {
 
    return mCameraMatrix;
}


void Camera::UpdateCameraMatrix() {
    UpdateLocalTransform();
    UpdateGlobalTransform();
    glm::vec4 globalPosition = globalTransform * glm::vec4(mTransform.position + mDragVector, 1);

    /*
    if(mIsDragged) {
        std::cout << " Drag vector " << glm::to_string(mDragVector) << " local Pos " << to_string(mTransform.position + mDragVector) << std::endl;//glm::to_string(globalPosition) << std::endl;
    } */

    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(globalPosition), glm::vec3(globalPosition) + glm::vec3(0, 0, 1), glm::vec3(0,1,0));
    mCameraMatrix = mProjectionMatrix * viewMatrix;//* globalTransform;
}

void Camera::SetupProjectionMatrix() {

    if(mOrthographic) {
        mProjectionMatrix = ortho(-((float) mViewport.width/2.0f),((float) mViewport.width/2.0f), -((float) mViewport.height/2.0f),((float) mViewport.height/2.0f), 0.1f, 10000.0f);
    } else{
        mProjectionMatrix = perspective(radians(60.0f), (float)mViewport.width / mViewport.height, 0.1f, 10000.0f);
    }
}

void Camera::ApplyDrag() {


    Translate(mDragVector.x / 2, mDragVector.y / 2, mDragVector.z); //TODO THIS STRANGE BUG NEEDS TO BE FIXED
    mDragVector = glm::vec3(0);

    mIsDragged = false;

    UpdateCameraMatrix();
}
