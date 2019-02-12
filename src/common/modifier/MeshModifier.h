#pragma once
#include <stdio.h>
#include "EditableMesh.h"

class MeshModifier {
public:
    MeshModifier() : mMesh(nullptr) {};
    virtual void BindMesh(EditableMesh *mesh) = 0;
    virtual void ComputeModifier() = 0;

protected:
    EditableMesh *mMesh;
};
