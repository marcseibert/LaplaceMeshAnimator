
#ifndef LAPLACEMESHANIMATOR_COMMONSTRUCTS_H
#define LAPLACEMESHANIMATOR_COMMONSTRUCTS_H

#include <glm/glm.hpp>

using namespace glm;
typedef struct {
    vec3 position;
    vec3 normal;
    vec2 texCoords;
} Vertex;

typedef struct R {

    R() { };
    R(float x, float y, float width, float height) :
    position(glm::vec2(x,y)),
    width(width),
    height(height) { };

    vec2 position;
    float width, height;
} Rect;

typedef struct {

} Texture;
#endif //LAPLACEMESHANIMATOR_COMMONSTRUCTS_H
