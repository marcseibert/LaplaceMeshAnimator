//
// Created by Marc Seibert on 2019-02-11.
//

#ifndef LAPLACEMESHANIMATOR_EDITABLEMODEL_H
#define LAPLACEMESHANIMATOR_EDITABLEMODEL_H
#include "../drawables/Model.h"
#include "EditableMesh.h"
#include <string>

class EditableModel : public RenderIDObject, public RenderObject {
public:
    EditableModel() = default;
    EditableModel(Model &model, unsigned int id = 0) : RenderIDObject(id), mSelected(false) {
        mModel = &model;
        auto &meshes = *mModel->GetMeshes();

        for(auto &mesh : meshes){//auto iter = meshes.begin(); iter != meshes.end(); iter++){//auto &mesh : meshes) {
            mMeshes.push_back(EditableMesh(mesh, mMeshes.size()));
        }
    };

    void Draw(Camera &camera) override {

        for(auto &mesh : mMeshes) {
            mesh.Draw(camera);
        }
    };

    void DrawID(Camera &camera) override {
        for(auto &mesh : mMeshes) {
            mesh.DrawID(camera, ID);
        }
    };

    void SetSelected(bool b) {
        mSelected = b;

        for(auto &mesh : mMeshes) {
            mesh.SetActive(b);
        }
    }

protected:
    Model *mModel;
    bool mSelected;
    std::vector<EditableMesh> mMeshes;
};

#endif //LAPLACEMESHANIMATOR_EDITABLEMODEL_H
