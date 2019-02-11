//
// Created by Marc Seibert on 2019-02-11.
//

#ifndef LAPLACEMESHANIMATOR_INTERSECTIONMESH_H
#define LAPLACEMESHANIMATOR_INTERSECTIONMESH_H
#include <glm/glm.hpp>

class IntersectionMesh {
public:
    virtual bool RayIntersects(glm::vec4 ray) = 0;
    virtual glm::vec3 RayIntersectsAt(glm::vec4 ray) = 0;
};
#endif //LAPLACEMESHANIMATOR_INTERSECTIONMESH_H
