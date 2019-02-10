//
// Created by Marc Seibert on 2019-02-08.
//

#ifndef LAPLACEMESHANIMATOR_SCENENODE_H
#define LAPLACEMESHANIMATOR_SCENENODE_H
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <list>
#include <iostream>
#include <vector>

typedef struct T {

    T() :position(glm::vec3(0)), scale(glm::vec3(1)), rotation(glm::vec3(0)){};
    T(glm::vec3 position) :position(position), scale(glm::vec3(1)), rotation(glm::vec3(0)){};

    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
} Transform;

class SceneNode {
public:
    SceneNode() : mChildren(), localTransform(1), globalTransform(1), mParent(nullptr) { mChildren.resize(0); }
    // INIT WITH POSITION
    SceneNode(float x, float y, float z) : mChildren(), localTransform(1), globalTransform(1), mTransform(glm::vec3(x, y, z)), mParent(
            nullptr) { mChildren.resize(0);}

    std::vector<SceneNode*>::const_iterator GetChildren() {
        return mChildren.cbegin();
    }

    SceneNode* GetParent(){ return mParent; };

    void SetPosition(float x, float y, float z) {
        mTransform.position = glm::vec3(x, y, z);
        UpdateLocalTransform();
    };

    void Translate(float x, float y, float z) {
        mTransform.position += glm::vec3(x, y, z);

        UpdateLocalTransform();
    };

    void SetScale(float x, float y, float z) {
        mTransform.scale = glm::vec3(x, y, z);
        UpdateLocalTransform();
    };

    void Scale(float x, float y, float z) {
        mTransform.scale *= glm::vec3(x, y, z);
        UpdateLocalTransform();
    };

    void SetRotation(float eulerX, float eulerY, float eulerZ) {
        // TODO: IMPLEMENT ROTATION
        UpdateLocalTransform();
    };

    void Rotate(glm::vec3 axix, float angle) {
        // TODO: IMPLEMENT ROTATION
        UpdateLocalTransform();
    };

    void UpdateGlobalTransform() {

        if(!mParent) {
            globalTransform = localTransform;
        } else {
            globalTransform = mParent->globalTransform * localTransform;
        }

        for(std::vector<SceneNode*>::iterator child = mChildren.begin(); child != mChildren.end(); child++)
        {
            if(*child) {

                (*child)->UpdateGlobalTransform();
            }
        }
    };

    void SetParent(SceneNode *parent) {
        mParent = parent;

        if(mParent) {
            parent->AddChild(*this);
        }
    };

    void AddChild(SceneNode &child) {

        std::cout << "Add child " << mChildren.size() <<  std::endl;
        //TODO: DOES NOT CHECK IF CHILD IS ALREADY IN LIST
        mChildren.push_back(&child);
        child.mParent = this;
    };


    virtual void Update(GLFWwindow *window, float deltaTime) = 0;
    virtual ~SceneNode() = default;

protected:
    Transform mTransform;

    glm::mat4 localTransform;
    glm::mat4 globalTransform;

    std::vector<SceneNode*> mChildren;
    SceneNode *mParent;

    void UpdateLocalTransform() {
        glm::mat4 transform = glm::mat4(1);

        //transform = glm::rotate(transform, )
        transform = glm::translate(transform, mTransform.position);
        transform = glm::scale(transform, mTransform.scale);

        localTransform = transform;
    };
};
#endif //LAPLACEMESHANIMATOR_SCENENODE_H
