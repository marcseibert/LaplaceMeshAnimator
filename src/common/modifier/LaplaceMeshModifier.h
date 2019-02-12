//
// Created by Marc Seibert on 2019-02-12.
//

#ifndef LAPLACEMESHANIMATOR_LAPLACEMESHMODIFIER_H
#define LAPLACEMESHANIMATOR_LAPLACEMESHMODIFIER_H
#include "MeshModifier.h"
#include "MeshMatrix.h"
#include "EditableModel.h"
#include <Eigen/Sparse>
#include <Eigen/SparseCholesky>
#include <Eigen/LU>
#include <glm/glm.hpp>
#include <vector>

using namespace Eigen;
class LaplaceMeshModifier : public MeshModifier {
public:
    explicit LaplaceMeshModifier(EditableModel *model) : MeshModifier() {

    };

    void BindMesh(EditableMesh *mesh) override {
        typedef Eigen::Triplet<float> T;

        mMesh = mesh;
        mVertexCount = mMesh->mMesh->mVertices.size();
        mMeshMatrix = MeshMatrix(*mesh->mMesh);

        ComputeLaplaceOperator();
        ComputeLaplaceCoords();
        ComputeTransformOperator();


        // PRE COMPUTE
    };

    void ComputeModifier() override {
        auto &constraints = *mMesh->GetSelectedVertices();
        SetConstraints(constraints);

        // CONSTRUCT RESULT VECTOR
        auto &vertices = mMesh->mMesh->mVertices;
        int constraintCount = constraints.size();
        int bSize = mVertexCount + constraintCount;

        VectorXf b((bSize) * 3);
        for(int i = 0; i < mVertexCount; i++) {
            b(i)           = mLaplaceCoords(i, 0);
            b(i +   bSize) = mLaplaceCoords(i, 1);
            b(i + 2*bSize) = mLaplaceCoords(i, 2);
        }

        int constraintIterCount = mVertexCount;
        for(std::list<unsigned int>::iterator it = constraints.begin(); it != constraints.end(); ++it) {
            b(constraintIterCount          ) = vertices[*it].Position.x;
            b(constraintIterCount +   bSize) = vertices[*it].Position.y;
            b(constraintIterCount + 2*bSize) = vertices[*it].Position.z;
            constraintIterCount++;
        }

        VectorXf x;
        x = mSolver.solve(mTransposeMatrixA * b);
    };

    void SetConstraints(std::list<unsigned int> &constraints) {
        typedef Triplet<float> T;

        int constraintCount = constraints.size();
        int bSize = mVertexCount + constraintCount;

        // CONSTRUCT OPERATOR
        Eigen::SparseMatrix<float> matrixA(mVertexCount, (mVertexCount + constraintCount) * 3);
        std::vector<T> inserts;
        inserts.reserve(9*(mVertexCount + constraintCount));

        // INSERT LAPLACE OPERATOR
        for(int row = 0; row < mVertexCount; row++) {
            for(SparseMatrix<float>::InnerIterator it(mLaplaceOperator, row); it; ++it) {
                inserts.push_back(T(row,                  it.row(), it.value()));
                inserts.push_back(T(row,   mVertexCount + it.row(), it.value()));
                inserts.push_back(T(row, 2*mVertexCount + it.row(), it.value()));
            }
        }

        // INSERT CONSTRAINT IDENTITY
        for(int i = 0; i < constraintCount; i++) {
            inserts.push_back(T(             mVertexCount + i    , mVertexCount + i    , 1));
            inserts.push_back(T(bSize      + mVertexCount + i + 1, mVertexCount + i + 1, 1));
            inserts.push_back(T(bSize * 2  + mVertexCount + i + 2, mVertexCount + i + 2, 1));
        }

        matrixA.setFromTriplets(inserts.begin(), inserts.end());


        // INSERT LAPLACE OPERATOR THREE TIMES
        // INSERT CONDITION VECTOR THREE TIMES

        mTransposeMatrixA = matrixA.transpose();

        // SOLVE USING NORMAL EQUATION
        mSolver.compute(mTransposeMatrixA * matrixA);
    };
private:
    int mVertexCount;
    MeshMatrix mMeshMatrix;
    Eigen::Matrix<float, Eigen::Dynamic, 3> mLaplaceCoords;
    Eigen::SparseMatrix<float> mTransposeMatrixA;
    Eigen::SparseMatrix<float> mLaplaceOperator;


    SimplicialLDLT<SparseMatrix<float>> mSolver;

    // TAKE CARE THE RESULT IS A Nx3 MATRIX
    // WE ARE LATER USING A N*3 VECTOR
    void ComputeLaplaceOperator() {
        typedef Eigen::Triplet<float> T;

        std::vector<T> tripletList;
        tripletList.reserve(mVertexCount * 3);

        auto &adjMatrix = mMeshMatrix.AdjacencyMatrix;
        auto &valence = mMeshMatrix.Valence;
        mLaplaceOperator.resize(mVertexCount, mVertexCount);
        for(int row = 0; row < mVertexCount; row++) {
            // USING THE LAPLACE DEFINITION FROM THE PAPER
            tripletList.push_back(T(row, row, 1));

            for(SparseMatrix<unsigned char>::InnerIterator it(adjMatrix, row); it; ++it) {
                if(row != it.row()) {
                    tripletList.push_back(T(row, it.row(), -1.0f / valence[row]));
                }
            }
        }
        mLaplaceOperator.setFromTriplets(tripletList.begin(), tripletList.end());
    }

    void ComputeLaplaceCoords() {
        auto &mesh = *mMesh->mMesh;

        auto &vertices = mesh.mVertices;
        Matrix<float, Dynamic, 3> coords(mVertexCount, 3);

        for(int i = 0; i < mVertexCount; i++){
            coords(i, 0) = vertices[i].Position.x;
            coords(i, 1) = vertices[i].Position.y;
            coords(i, 2) = vertices[i].Position.z;
        }

        mLaplaceCoords = mLaplaceOperator * coords;
    }

    void ComputeTransformOperator() {/*
        auto &mesh = *mMesh->mMesh;
        auto &adjMatrix = mMeshMatrix.AdjacencyMatrix;

        typedef Triplet<float> T;

        SparseMatrix<float> tOperator(6 * mVertexCount, 3 * mVertexCount);
        std::vector<T> inserts(3 * mVertexCount);

        for(int i = 0; i < mVertexCount; i++) {
            // GENERATE MATRIX A
            std::vector<glm::vec3*> neighborhood; // N(v) u {v}

            neighborhood.push_back(&mesh.mVertices[i].Position); // ADD v

            for(Eigen::SparseMatrix<unsigned char>::InnerIterator it(adjMatrix, i); it; ++it) {
                if(it.row() != i) {
                    neighborhood.push_back(&mesh.mVertices[it.row()].Position);
                }
            }

            Matrix<float, Dynamic, 7> matrixA;

            //matrixA.resize(neighborhood.size());
            for(auto &vertex : neighborhood) {
                Matrix<float, 3, 7> perVertexData;

                // CREATING SKREW SYMMETRIC MATRIX h + position information + neighbors
                perVertexData << vertex->x,         0, vertex->z, -vertex->y, 1, 0, 0,
                                 vertex->y, -vertex->z,        0,  vertex->x, 0, 1, 0,
                                 vertex->z,  vertex->y, -vertex->x,        0, 0, 0, 1;

                matrixA.block<3,7>(0, i * 3) = perVertexData;
            }

            Matrix<float, 7, Dynamic> adjustedEquation; // NEEDED TO SOLVE THE PARAMETERS OF the tOperator
            if(neighborhood.size() == 0) {
                adjustedEquation = MatrixXd::Zero(7, 3);
            } else {
                auto transposeA = matrixA.transpose();
                adjustedEquation = (transposeA * matrixA).inverse() * transposeA;
            }

            auto s  = adjustedEquation.row(0);
            auto h1 = adjustedEquation.row(1);
            auto h2 = adjustedEquation.row(2);
            auto h3 = adjustedEquation.row(3);
            auto t1 = adjustedEquation.row(4);
            auto t2 = adjustedEquation.row(5);
            auto t3 = adjustedEquation.row(6);

            Matrix<float, 4, 4> tOp;
            tOp <<    s, -h3,  h2, t1,
                     h3,   s, -h1, t2,
                    -h2,  h1,   s, t3,
                      0,   0,   0,  1;

            // ... DER OPERATOR MUESSTE NOCH ZWISCHEN GESPEICHERT WERDEN. PROBLEM: Der operator produziert hom Coords..

        }*/
    }

    void ComputeConstraints(SparseMatrix<float> &constraints) {
        typedef Triplet<unsigned char> T;

        auto &selected = *mMesh->GetSelectedVertices();

        //constraints.reserve(selected.size() * 3,mVertexCount);
        std::vector<T> insert(selected.size() * 3);

        for(auto &vertexID : selected) {
            insert.push_back(T(vertexID    , vertexID    , 1.0f));
            insert.push_back(T(vertexID + 1, vertexID + 1, 1.0f));
            insert.push_back(T(vertexID + 2, vertexID + 2, 1.0f));
        }

        constraints.setFromTriplets(insert.begin(), insert.end());
    }
};
#endif //LAPLACEMESHANIMATOR_LAPLACEMESHMODIFIER_H
