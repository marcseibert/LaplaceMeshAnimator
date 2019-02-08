#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Camera {
private:

    mat4 projectionMatrix;
    mat4 viewMatrix;
    
    mat4 cameraMatrix;
    
    void UpdateCameraMatrix();
public:
    vec3 cameraPosition; // WORLD COORDINATES
    Camera() {};
    Camera(int screenWidth, int screenHeight, float x, float y, float z, bool orthographic=false);
    
    mat4 GetCameraMatrix();
    void ApplyViewportChange(GLFWwindow* window, int width, int height);
    
    void SetRotation(float pitch, float yaw);
    void SetPosition(float x, float y, float z);
    void Translate(float x, float y, float z);

};
