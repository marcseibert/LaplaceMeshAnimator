//
// Created by Marc Seibert on 2019-02-09.
//
#pragma once
#include "Camera.hpp"
#include <glm/glm.hpp>

class DraggableObject {
public:
    DraggableObject() : mIsDragged(false), mDragVector(0) { };

    virtual void ApplyDrag() = 0;

    virtual void Drag(glm::vec3 dragVector)  {
        mDragVector = dragVector;
        mIsDragged = true;
    };

    bool IsDragged() { return mIsDragged; };
protected:
    glm::vec3 mDragVector;
    bool mIsDragged;
};
