#pragma once
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../SceneNode.h"
#include "../RenderObject.h"
#include "../Shader.h"
#include "../Util.h"
#include "../Camera.h"
#include "Mesh.h"

class Model : public SceneNode, public RenderObject
{
public:
    Model() = default;
    Model(std::string const &path, float x, float y, float z, bool gamma = false) : SceneNode(x, y, z), RenderObject(), gammaCorrection(gamma) {

        directory = path.substr(directory[0], directory.find_last_of("/"));
        loadModel(path);
        UpdateGlobalTransform();
    }

    void Draw(Camera &camera) override;
    void Update(GLFWwindow *window, float deltaTime) override;

    std::list<Mesh>* GetMeshes() { return &meshes; };
protected:
    /*  Model Data */
    std::vector<Texture> textures_loaded;    // stores all the mTextures loaded so far, optimization to make sure mTextures aren't loaded more than once.
    std::list<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

    void loadModel(std::string const &path);
    virtual void processNode(aiNode *node, SceneNode *parent, const aiScene *scene);
    virtual Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};
