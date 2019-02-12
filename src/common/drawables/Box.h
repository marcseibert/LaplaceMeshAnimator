#ifndef Box_hpp
#define Box_hpp

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <string>

#include "../Camera.h"
#include "../ShaderManager.h"
#include "../SceneNode.h"
#include "../RenderObject.h"

using namespace std;
using namespace glm;


class Box : public SceneNode, public RenderObject {
public:

    Box() : Box(Rect(0,0,1,1), glm::vec4(1)) {};
    Box(Rect rect, glm::vec4 color);

    void Draw(Camera &camera) override;
    void Update(GLFWwindow *window, float deltaTime) override;
    //void SetOffsetRect(float x, float y, float width, float height);
    ~Box() override;
    
private:
    Rect bounds;
    GLuint VAO, VBO, EBO;
    float offsetX, offsetY, offsetWidth, offsetHeight;
    mat4 modelMatrix;
    vec3 mColor;

    void UpdateModelMatrix(Rect viewport);

};
#endif /* Sprite_hpp */
