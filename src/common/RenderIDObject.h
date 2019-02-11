//
// Created by Marc Seibert on 2019-02-11.
//

#ifndef LAPLACEMESHANIMATOR_RENDERIDOBJECT_H
#define LAPLACEMESHANIMATOR_RENDERIDOBJECT_H
#include "Camera.h"

class RenderIDObject {
public:
    unsigned int ID;

    RenderIDObject(unsigned int id) : ID(id) { };
    virtual void DrawID(Camera &camera) = 0;
};

#endif //LAPLACEMESHANIMATOR_RENDERIDOBJECT_H
