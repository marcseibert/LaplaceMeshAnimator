//
// Created by Marc Seibert on 2019-02-08.
//

#ifndef LAPLACEMESHANIMATOR_SCENENODE_H
#define LAPLACEMESHANIMATOR_SCENENODE_H
#include <list>
#include <glm/glm.hpp>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

typedef struct T {

    T() :position(glm::vec3(0)), scale(glm::vec3(1)), rotation(glm::vec3(0)){};
    T(glm::vec3 position) :position(position), scale(glm::vec3(1)), rotation(glm::vec3(0)){};

    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
} Transform;

class SceneNode {
public:
    SceneNode() : mChildren() { mChildren.resize(0); }

    std::list<SceneNode*>::const_iterator GetChildren() {
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

        globalTransform = mParent->globalTransform * localTransform;

        for(auto &child : mChildren) {
            child->UpdateGlobalTransform();
        }
    };
    void SetParent(SceneNode *parent) {
        mParent = parent;

        parent->AddChild(this);
    };

    void AddChild(SceneNode *child) {
        //TODO: DOES NOT CHECK IF CHILD IS ALREADY IN LIST
        mChildren.push_back(child);
        child->mParent = this;
    };


    virtual void Update() = 0;
    virtual ~SceneNode() = default;

protected:
    Transform mTransform;

    glm::mat4 localTransform;
    glm::mat4 globalTransform;

    std::list<SceneNode*> mChildren;
    SceneNode *mParent;

    void UpdateLocalTransform() {
        glm::mat4 transform = glm::mat4(1);

        //transform = glm::rotate(transform, )
        transform = glm::scale(transform, mTransform.scale);
        transform = glm::translate(transform, mTransform.position);

        localTransform = transform;
        // ROTATION
        // SCALE
        // TRANSLATE
    };
};
#endif //LAPLACEMESHANIMATOR_SCENENODE_H
