#ifndef Drawable_hpp
#define Drawable_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include "../Shader.hpp"
#include "../Camera.hpp"

using namespace glm;

struct Transform{
    vec3 position;
    vec3 scale;
    vec3 rotation;
};

typedef struct {
    float x, y;
    float width;
    float height;
} Rectangle;

class Drawable {

public:
    Drawable() : Drawable(vec3(0,0,0)) {};
    Drawable(float x, float y, float z) : Drawable(vec3(x,y,z)) {};
    Drawable(vec3 position) {
        transform.position = position;
        transform.scale = vec3(1);
        transform.rotation = vec3(0);
    };
    virtual ~Drawable() = default;
    
    virtual void Draw(Camera &camera) = 0;
    
protected:
    Transform transform;
};
#endif /* Drawable_hpp */
