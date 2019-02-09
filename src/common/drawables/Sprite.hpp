#ifndef Sprite_hpp
#define Sprite_hpp

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <string>

#include "../CommonStructs.h"
#include "../ShaderManager.hpp"
#include "../RenderObject.h"
#include "../SceneNode.h"

using namespace std;
using namespace glm;

class Sprite : public SceneNode, public RenderObject {
public:
    Sprite() : SceneNode(), RenderObject() {};
    
    Sprite(string const &path) : Sprite(path, 0,0,0,1,1) {};
    Sprite(string const &path, float x, float y, float z, float width, float height);
    
    void Draw(Camera &camera) override;
    //void SetOffsetRect(float x, float y, float width, float height);
    ~Sprite() override;
    
private:
    Rect mBounds;
    GLuint VAO, VBO, EBO, textureID;
    float offsetX, offsetY, offsetWidth, offsetHeight;
    mat4 mSpriteMatrix;

    void LoadTextureFromFile(string const &path);
    void UpdateModelMatrix();

};
#endif /* Sprite_hpp */
