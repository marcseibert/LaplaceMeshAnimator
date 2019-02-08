
#ifndef LAPLACEMESHANIMATOR_COMMONSTRUCTS_H
#define LAPLACEMESHANIMATOR_COMMONSTRUCTS_H

#include <glm/glm.hpp>

using namespace glm;
typedef struct {
    vec3 position;
    vec3 normal;
    vec2 texCoords;
} Vertex;

typedef struct {
    vec2 position;

    float width, height;
} Rect;

#endif //LAPLACEMESHANIMATOR_COMMONSTRUCTS_H
