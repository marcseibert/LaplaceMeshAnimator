//
// Created by Marc Seibert on 2019-02-08.
//

#ifndef LAPLACEMESHANIMATOR_MESH_H
#define LAPLACEMESHANIMATOR_MESH_H
#include "GL/glew.h"

#include <vector>
#include <glm/glm.hpp>
#include <Eigen/SparseCore>
#include "CommonStructs.h"

class Mesh {
public:
    Mesh() = default;

    Mesh(std::vector<glm::vec3> verticies, std::vector<glm::vec3> faces, std::vector<Texture> textures);


    std::vector<glm::vec3> GetVerticies();
    std::vector<glm::vec3> GetFaces();
private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> triangles;

    GLuint VAO, VBO, EBO;

    void setupMesh();
};

class MeshMatrix {
public:
    MeshMatrix() = default;

    MeshMatrix(Mesh &mesh) : adjacencyMatrix(mesh.GetVerticies().size(), mesh.GetVerticies().size()){
        //std::vector<vec3> v
        for(auto &triangle : mesh.GetFaces()) {

        };
    };

    Eigen::SparseMatrix<unsigned char> *GetAdjacencyMatrix();

private:
    Eigen::SparseMatrix<unsigned char> adjacencyMatrix;
};
#endif //LAPLACEMESHANIMATOR_MESH_H