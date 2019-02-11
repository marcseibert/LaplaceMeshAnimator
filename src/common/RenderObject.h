//
// Created by Marc Seibert on 2019-02-09.
//

#ifndef LAPLACEMESHANIMATOR_RENDEROBJECT_H
#define LAPLACEMESHANIMATOR_RENDEROBJECT_H
#include "Camera.h"

class RenderObject {
public:
    RenderObject() = default;
    virtual void Draw(Camera &camera) = 0;
};

#endif //LAPLACEMESHANIMATOR_RENDEROBJECT_H
