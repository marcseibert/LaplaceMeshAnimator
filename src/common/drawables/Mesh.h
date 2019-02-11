//
// Created by Marc Seibert on 2019-02-08.
//

#ifndef LAPLACEMESHANIMATOR_MESH_H
#define LAPLACEMESHANIMATOR_MESH_H
#include "GL/glew.h"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Eigen/SparseCore>
#include "../CommonStructs.h"
#include "../SceneNode.h"
#include "../RenderObject.h"
#include "../Camera.h"
#include "../ShaderManager.h"
#include <GLFW/glfw3.h>
#include "../collision/IntersectionMesh.h"

class Mesh : public SceneNode, public RenderObject, public IntersectionMesh {
public:
    Mesh() = default;

    Mesh(std::vector<Vertex> &verticies, std::vector<glm::uvec3> &faces, std::vector<Texture> &textures);

    void Update(GLFWwindow *window, float deltaTime) override;
    void Draw(Camera &camera) override;

    std::vector<Vertex> *GetVertices();
    std::vector<glm::uvec3> *GetFaces();

    bool RayIntersects(glm::vec4 ray) override;

    glm::vec3 RayIntersectsAt(glm::vec4 ray) override;
private:
    std::vector<Vertex> vertices;
    std::vector<glm::uvec3> faces;
    std::vector<Texture> textures;

    GLuint VAO, VBO, EBO;

    void setupMesh();
};

class MeshMatrix {
public:
    MeshMatrix() = default;

    MeshMatrix(Mesh &mesh) : adjacencyMatrix(mesh.GetVertices()->size(), mesh.GetVertices()->size()){

        for(auto &triangle : *mesh.GetFaces()) {

        };
    };

    Eigen::SparseMatrix<unsigned char> *GetAdjacencyMatrix();

private:
    Eigen::SparseMatrix<unsigned char> adjacencyMatrix;
};
#endif //LAPLACEMESHANIMATOR_MESH_H