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

        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS) {
            if(!mPressed[MOUSE_BUTTON_CENTER]) {
                mDragOrigin = mPosition;

                mPressed[MOUSE_BUTTON_CENTER] = true;
                mDrag = false;
            }
        } else {
            mPressed[MOUSE_BUTTON_CENTER] = false;
            mDrag = false;
        }
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

    bool IsPressed(ButtonType button) {
        return mPressed[button];
    }

    glm::dvec2 GetPosition() {
        return ClampPosition(mPosition);
    };

    glm::dvec2 GetDrag() {
        return ClampPosition(mDragVector);
    };


private:
    glm::dvec2 mPosition;
    glm::dvec2 mDragVector;
    float mDragSensitivity;
    float mScreenWidth, mScreenHeight;
    Rect mViewport;
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
