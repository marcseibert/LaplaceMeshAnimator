//
//  MeshOperator.cpp
//  Globe
//
//  Created by Marc Seibert on 08.02.19.
//  Copyright © 2019 Marc Seibert. All rights reserved.
//

#include <stdio.h>
#include "../Mesh.h"

class MeshModifier {
    
    MeshModifier(Mesh)
    virtual void bindObject(Mesh &mesh) = 0;
    virtual void bindConstraints() = 0;
    virtual void computeOperator() = 0;
};
