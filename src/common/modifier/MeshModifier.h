//
//  MeshOperator.cpp
//  Globe
//
//  Created by Marc Seibert on 08.02.19.
//  Copyright © 2019 Marc Seibert. All rights reserved.
//

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
