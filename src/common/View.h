//
// Created by Marc Seibert on 2019-02-08.
//

#ifndef LAPLACEMESHANIMATOR_VIEW_H
#define LAPLACEMESHANIMATOR_VIEW_H
#include "CommonStructs.h"
#include "SceneNode.h"
#include "RenderObject.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

class View : public SceneNode{
public:

    View() : SceneNode(), mDirty(false){ };

    View(float x, float y, float width, float height) : SceneNode(), mBounds(Rect(x, y, width, height)), mDirty(false) { };

    Rect GetBounds() { return mBounds; };

    void SetClearColor(glm::vec4 clearColor) {
        mClearColor = clearColor;
    }

    void Repaint() {
        mDirty = true;
    };

    virtual void Draw() {
        SetViewport();

        glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mDirty = false;
    };

    void Update() override {

    };

    bool IsDirty() { return mDirty; };
    ~View() override {
    };
protected:
    Rect mBounds;
    glm::vec4 mClearColor;
    bool mDirty;

    void SetViewport() {
        glScissor(mBounds.position.x, mBounds.position.y, mBounds.width, mBounds.height);
        glViewport(mBounds.position.x, mBounds.position.y, mBounds.width, mBounds.height);
    };
};


#endif //LAPLACEMESHANIMATOR_VIEW_H
