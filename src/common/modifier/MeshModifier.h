#pragma once
#include <stdio.h>
#include "EditableMesh.h"

class MeshModifier {
public:
    MeshModifier() : mMesh(nullptr), mEnabled(false) {};
    virtual void BindMesh(EditableMesh *mesh) = 0;
    virtual void ComputeModifier() = 0;

    virtual void Enable() = 0;
    virtual void Disable() = 0;

protected:
    EditableMesh *mMesh;
    bool mEnabled;
};
