#include "Camera.hpp"

Camera::Camera(int screenWidth, int screenHeight, float x, float y, float z, bool orthographic) {
    this->cameraPosition = vec3(x, y, z);
    this->viewMatrix = translate(mat4(1), this->cameraPosition);
    
    if(orthographic) {
        this->projectionMatrix = ortho(-((float) screenWidth/2.0f),((float) screenWidth/2.0f), -((float) screenHeight/2.0f),((float) screenHeight/2.0f), 0.1f, 100.0f);
    } else{
        this->projectionMatrix = perspective(radians(45.0f), (float)screenWidth / screenHeight, 0.1f, 100.0f);
    }

    UpdateCameraMatrix();
}

void Camera::ApplyViewportChange(GLFWwindow* window, int width, int height) {
    
}

void Camera::SetRotation(float pitch, float yaw) {
    
}

void Camera::SetPosition(float x, float y, float z) {
    this->cameraPosition = vec3(x, y, z);
}

void Camera::Translate(float x, float y, float z) {
    this->cameraPosition += vec3(x,y,z);
    this->viewMatrix = translate(this->viewMatrix, vec3(x,y,z));
    
    UpdateCameraMatrix();
}

mat4 Camera::GetCameraMatrix() {
 
    return cameraMatrix;
}


void Camera::UpdateCameraMatrix() {
    cameraMatrix = projectionMatrix * viewMatrix;
}
