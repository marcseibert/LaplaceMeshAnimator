//
// Created by Marc Seibert on 2019-02-12.
//

#ifndef LAPLACEMESHANIMATOR_MESHMATRIX_H
#define LAPLACEMESHANIMATOR_MESHMATRIX_H
#include <Eigen/Sparse>
#include "../drawables/Mesh.h"
#include <set>
#include <list>


using namespace Eigen;
class MeshMatrix {
public:
    MeshMatrix() = default;

    MeshMatrix(Mesh &mesh)
    : AdjacencyMatrix(mesh.GetVertices()->size(), mesh.GetVertices()->size()) {

        typedef Eigen::Triplet<unsigned char> T;

        std::vector<T> tripletList;
        tripletList.reserve(3 * mesh.GetVertices()->size());
        Valence.resize(mesh.GetVertices()->size());

        for (auto &triangle : *mesh.GetFaces()) {
            tripletList.push_back(T(triangle.x, triangle.y, 1));
            tripletList.push_back(T(triangle.y, triangle.x, 1));

            tripletList.push_back(T(triangle.y, triangle.z, 1));
            tripletList.push_back(T(triangle.z, triangle.y, 1));

            tripletList.push_back(T(triangle.z, triangle.x, 1));
            tripletList.push_back(T(triangle.x, triangle.z, 1));
        }
        AdjacencyMatrix.setFromTriplets(tripletList.begin(), tripletList.end());

        for(int col = 0; col < AdjacencyMatrix.cols(); col++){
            Valence[col] = AdjacencyMatrix.col(col).nonZeros();
        }
    };

    std::list<unsigned int> GetVerticesInRange(unsigned int vertex, int range) {
        std::set<unsigned int> reachableVertices;
        std::set<unsigned int> newVertices;

        reachableVertices.insert(vertex);
        for(int i = 0; i < range; i++) {
            newVertices.clear();
            for(auto &vert : reachableVertices) {
                for(SparseMatrix<unsigned char>::InnerIterator it(AdjacencyMatrix, vert); it; ++it) {
                    newVertices.insert(it.row());
                }
            }

            reachableVertices.insert(newVertices.begin(), newVertices.end());
        }

        return std::list<unsigned int>(reachableVertices.begin(), reachableVertices.end());
    }

    std::vector<int> Valence;
    SparseMatrix<unsigned char> AdjacencyMatrix;
};

#endif //LAPLACEMESHANIMATOR_MESHMATRIX_H
