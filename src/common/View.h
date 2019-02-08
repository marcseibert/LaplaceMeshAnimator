//
// Created by Marc Seibert on 2019-02-08.
//

#ifndef LAPLACEMESHANIMATOR_VIEW_H
#define LAPLACEMESHANIMATOR_VIEW_H
#include "CommonStructs.h"
#include "SceneNode.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

class View : public SceneNode {
public:

    View() : SceneNode() { };

    View(float x, float y, float width, float height) : SceneNode(), mBounds(Rect(x, y, width, height)) { };

    Rect GetBounds() { return mBounds; };

    void SetClearColor(glm::vec4 clearColor) {
        mClearColor = clearColor;
    }

    void Draw() {
        isDirty = false;
        glScissor(mBounds.position.x, mBounds.position.y, mBounds.width, mBounds.height);
        glViewport(mBounds.position.x, mBounds.position.y, mBounds.width, mBounds.height);
        glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(auto &child : mChildren) {
            child->Draw(glm::mat4(1));
        }
    };

    void Draw(glm::mat4 parentTransform) override { Draw(); };

    ~View() override {

    };
private:
    Rect mBounds;
    glm::vec4 mClearColor;
};


#endif //LAPLACEMESHANIMATOR_VIEW_H
