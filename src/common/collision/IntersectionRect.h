//
// Created by Marc Seibert on 2019-02-11.
//

#ifndef LAPLACEMESHANIMATOR_INTERSECTIONRECT_H
#define LAPLACEMESHANIMATOR_INTERSECTIONRECT_H

#include <glm/glm.hpp>

class IntersectionRect {
public:
    virtual bool InsideRect(glm::vec3 point) = 0;
};

#endif //LAPLACEMESHANIMATOR_INTERSECTIONRECT_H
