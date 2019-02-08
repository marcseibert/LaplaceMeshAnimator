//
// Created by Marc Seibert on 2019-02-08.
//

#ifndef LAPLACEMESHANIMATOR_SCENENODE_H
#define LAPLACEMESHANIMATOR_SCENENODE_H
#include <list>
#include <glm/glm.hpp>
#include <iostream>

class SceneNode {
public:
    SceneNode() : SceneNode(nullptr) { };
    SceneNode(SceneNode *root) : mChildren(), mRoot(root), isDirty(false) { mChildren.resize(0); }

    std::list<SceneNode*>::const_iterator GetChildren() {
        return mChildren.cbegin();
    }

    SceneNode* GetParent(){ return mParent; };

    void SetParent(SceneNode *parent) {
        mParent = parent;
    };

    void AddChild(SceneNode &child) {
        mChildren.push_back(&child);
    };

    // FORCES ROOT NODE TO REPAINT THE WHOLE SUBTREE
    void Repaint() {
        if(mRoot != nullptr) {
            mRoot->isDirty = true;
        } else {
            std::cout <<" REPAINT THIS BITCH " << std::endl;
            isDirty = true;
        }
    }

    bool IsDirty() { return isDirty; }
    virtual void Draw(glm::mat4 parentTransform) = 0;
    virtual ~SceneNode() = default;
protected:
    glm::mat4 mTransform;
    std::list<SceneNode*> mChildren;
    SceneNode *mParent;
    SceneNode *mRoot;
    bool isDirty;
};
#endif //LAPLACEMESHANIMATOR_SCENENODE_H
