
#ifndef LAPLACEMESHANIMATOR_COMMONSTRUCTS_H
#define LAPLACEMESHANIMATOR_COMMONSTRUCTS_H

#include <glm/glm.hpp>
#include <string>

typedef struct {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
} Vertex;

typedef struct {
    unsigned int id;
    std::string type;
    std::string path;
} Texture;

typedef struct R {

    R() { };
    R(float x, float y, float width, float height) :
    position(glm::vec2(x,y)),
    width(width),
    height(height) { };

    glm::vec2 position;
    float width, height;
} Rect;

#endif //LAPLACEMESHANIMATOR_COMMONSTRUCTS_H
