//
// Created by Marc Seibert on 2019-02-10.
//

#ifndef LAPLACEMESHANIMATOR_MOUSEINPUT_H
#define LAPLACEMESHANIMATOR_MOUSEINPUT_H
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../CommonStructs.h"

enum ButtonType {
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTON_CENTER,
    NUM_BUTTON_TYPES
};

class MouseInput {
public:
    MouseInput() { };
    MouseInput(int screenWidth, int screenHeight, Rect viewport, float dragSensitifity)
    : mViewport(viewport), mScreenWidth(screenWidth), mScreenHeight(screenHeight), mDragSensitivity(dragSensitifity) { };

    void Update(GLFWwindow *window) {
        // CHECK FOR NEW BUTTON PRESSES
        mNewPressed[MOUSE_BUTTON_LEFT]   = !mPressed[MOUSE_BUTTON_LEFT] && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
        mNewPressed[MOUSE_BUTTON_RIGHT]  = !mPressed[MOUSE_BUTTON_RIGHT] && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS;
        mNewPressed[MOUSE_BUTTON_CENTER] = !mPressed[MOUSE_BUTTON_CENTER] && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS;

        // BUTTON IS STILL PRESSED IF OLD AND NEW STATE ARE BOTH TRUE
        mPressed[MOUSE_BUTTON_LEFT]   = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
        mPressed[MOUSE_BUTTON_RIGHT]  = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS;
        mPressed[MOUSE_BUTTON_CENTER] = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS;

        if(mPressed[MOUSE_BUTTON_CENTER]) {
            if(mNewPressed[MOUSE_BUTTON_CENTER]) {
                mDragOrigin = mPosition;

                mDrag = false;
            }
        } else {
            mDrag = false;
        }

        // UPDATE MOUSE STATES
    };

    void OnMouseMove(GLFWwindow *window, double xPos, double yPos) {
        mPosition = glm::dvec2(xPos, yPos);

        if(mPressed[MOUSE_BUTTON_CENTER]) {
            mDragVector = mPosition - mDragOrigin;

            if(!mDrag) {
                mDrag = glm::length(mDragVector) > mDragSensitivity;
            }
        }

    };

    void SetWindowSize(float width, float height) {
        mScreenWidth = width;
        mScreenHeight = height;
    };

    void SetViewport(Rect viewport) {
        mViewport = viewport;
    };

    bool IsDragged() {
        return mDrag;
    };

    bool IsNewPressed(ButtonType button) {
        return mNewPressed[button];
    }

    bool IsPressed(ButtonType button) {
        return mPressed[button];
    }

    glm::dvec2 GetPosition() {
        return mPosition; //ClampPosition(mPosition);
    };

    glm::dvec2 GetDrag() {
        return mDragVector;//ClampPosition(mDragVector);
    };


private:
    glm::dvec2 mPosition;
    glm::dvec2 mDragVector;
    float mDragSensitivity;
    float mScreenWidth, mScreenHeight;
    Rect mViewport;

    bool mNewPressed[NUM_BUTTON_TYPES];
    bool mPressed[NUM_BUTTON_TYPES];

    bool mDrag;
    glm::dvec2 mDragOrigin;

    void OnDragRelease() {

    };

    void OnDragEnter(){

    };

    glm::dvec2 ClampPosition(glm::dvec2 pos) {
        glm::dvec2 clamped = pos - glm::dvec2(mViewport.position.x, mViewport.position.y);

        //clamped.x *= mScreenWidth / mViewport.width;
        //clamped.y *= mViewport.height / mScreenHeight;

        return clamped;
    };

};
#endif //LAPLACEMESHANIMATOR_MOUSEINPUT_H
