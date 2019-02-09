#include "Camera.hpp"

Camera::Camera(int screenWidth, int screenHeight, float x, float y, float z, bool orthographic) : SceneNode(), mOrthographic(orthographic) {



    UpdateCameraMatrix();
}

void Camera::ApplyViewportChange(GLFWwindow* window, int width, int height) {
    
}

void Camera::SetViewport(float x, float y, float width, float height) {
    mViewport = Rect(x, y, width, height);
    UpdateCameraMatrix();
}

void Camera::Update() {

}

mat4 Camera::GetCameraMatrix() {
 
    return mCameraMatrix;
}


void Camera::UpdateCameraMatrix() {

    mCameraMatrix = mProjectionMatrix * globalTransform;
}

void Camera::SetupProjectionMatrix() {

    if(mOrthographic) {
        mProjectionMatrix = ortho(-((float) mViewport.width/2.0f),((float) mViewport.width/2.0f), -((float) mViewport.height/2.0f),((float) mViewport.height/2.0f), 0.1f, 100.0f);
    } else{
        mProjectionMatrix = perspective(radians(45.0f), (float)mViewport.width / mViewport.height, 0.1f, 100.0f);
    }
}
