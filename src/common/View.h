//
// Created by Marc Seibert on 2019-02-08.
//

#ifndef LAPLACEMESHANIMATOR_VIEW_H
#define LAPLACEMESHANIMATOR_VIEW_H
#include <GL/glew.h>

#include "CommonStructs.h"
#include "SceneNode.h"
#include "RenderObject.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "input/MouseInput.h"
#include "collision/IntersectionRect.h"
class View : public SceneNode, public IntersectionRect {
public:

    View() : SceneNode(), mDirty(false), mActive(false) { };

    View(float x, float y, float width, float height, MouseInput *mouseInput)
    : SceneNode(), mBounds(Rect(x, y, width, height)), mDirty(false), mActive(false), mouseInput(mouseInput) { };

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

    void Update(GLFWwindow *window, float deltaTime) override {

    };

    void SetBounds(float x, float y, float width, float height){
        mBounds = Rect(x, y, width, height);
    };

    Rect GetViewport() { return mBounds; }
    void SetActive(bool b) {
        mActive = b;
    }

    bool IsActive() { return mActive; };
    bool IsDirty() { return mDirty; };

    // WORKS IN SCREEN SPACE
    bool InsideRect(glm::vec3 point) override {
        return mBounds.position.x <= point.x && (mBounds.position.x + mBounds.width) >= point.x
        && mBounds.position.y <= point.y && mBounds.position.y + mBounds.height >= point.y;
    };

    virtual void UpdateWindowParameters() = 0;
    ~View() override {
    };
protected:
    MouseInput *mouseInput;
    Rect mBounds;
    glm::vec4 mClearColor;
    bool mDirty;
    bool mActive;

    void SetViewport() {
        glScissor(mBounds.position.x, mBounds.position.y, mBounds.width, mBounds.height);
        glViewport(mBounds.position.x, mBounds.position.y, mBounds.width, mBounds.height);
    };
};


#endif //LAPLACEMESHANIMATOR_VIEW_H
