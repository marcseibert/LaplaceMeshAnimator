//
// Created by Marc Seibert on 2019-02-09.
//
#pragma once
#include "Camera.h"
#include <glm/glm.hpp>

class DraggableObject {
public:
    DraggableObject() : mIsDragged(false), mDragVector(0) { };

    virtual void ApplyDrag() = 0;

    virtual void Drag(glm::vec3 dragVector) = 0;

    bool IsDragged() { return mIsDragged; };
protected:
    glm::vec3 mDragVector;
    glm::vec3 mDeltaDrag;
    bool mIsDragged;
};
