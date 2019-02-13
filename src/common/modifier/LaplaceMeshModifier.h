//
// Created by Marc Seibert on 2019-02-12.
//

#ifndef LAPLACEMESHANIMATOR_LAPLACEMESHMODIFIER_H
#define LAPLACEMESHANIMATOR_LAPLACEMESHMODIFIER_H
#include "MeshModifier.h"
#include "MeshMatrix.h"
#include "../input/MouseInput.h"
#include "EditableModel.h"
#include <Eigen/Sparse>
#include <Eigen/SparseCholesky>
#include <Eigen/LU>
#include <glm/glm.hpp>
#include <vector>

using namespace Eigen;
class LaplaceMeshModifier : public MeshModifier {
public:
    explicit LaplaceMeshModifier() : MeshModifier() {
        mSolver = new SimplicialLDLT<SparseMatrix<float>>();
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
        // CONSTRUCT RESULT VECTOR
        auto &vertices = mMesh->mMesh->mVertices;
        int constraintCount = mConstraintPositions.size();
        int bSize = mVertexCount + constraintCount;

        VectorXf b(bSize * 3);

        for(int i = 0; i < mVertexCount; i++) {
            b(i)                  = mLaplaceCoords(i, 0);
            b(i +   mVertexCount) = mLaplaceCoords(i, 1);
            b(i + 2*mVertexCount) = mLaplaceCoords(i, 2);
        }

        int constraintIterCount = 3 * mVertexCount;
        for(std::vector<glm::vec3>::iterator it = mConstraintPositions.begin(); it != mConstraintPositions.end(); ++it) {
            b(constraintIterCount++) = it->x; // vertices[*it].Position.x;
            b(constraintIterCount++) = it->y; // vertices[*it].Position.y;
            b(constraintIterCount++) = it->z; // vertices[*it].Position.z;
        }

        //std::cout << "aT " << mTransposeMatrixA.rows() << "x" << mTransposeMatrixA.cols() << std::endl;
        VectorXf x;
        x = mSolver->solve(mTransposeMatrixA * b);

        if(mSolver->info()!= Eigen::Success) {
            // solving failed
            return;
        }
        //std::cout << x << std::endl;

        for(int i = 0; i < mVertexCount; i++) {
            vertices[i].Position = glm::vec3(x(i), x(mVertexCount + i), x(2*mVertexCount + i));
        }

        mMesh->mMesh->UpdateModel();
    };

    void SetConstraints(std::vector<unsigned int> &constraints) {
        typedef Triplet<float> T;

        int constraintCount = constraints.size();
        int bSize = mVertexCount + constraintCount;
        mConstIds = constraints;

        // CONSTRUCT OPERATOR
        Eigen::SparseMatrix<float> matrixA((mVertexCount + constraintCount)*3, mVertexCount * 3);
        std::vector<T> inserts;
        inserts.reserve(9*(mVertexCount + constraintCount));

        // INSERT LAPLACE OPERATOR
        for(int col = 0; col < mVertexCount; col++) {

            // ITERATRE OVER COLUMNS
            for(SparseMatrix<float>::InnerIterator it(mLaplaceOperator, col); it; ++it) {
                inserts.push_back(T(                 it.row(),                  col, it.value()));
                inserts.push_back(T(  mVertexCount + it.row(),   mVertexCount + col, it.value()));
                inserts.push_back(T(2*mVertexCount + it.row(), 2*mVertexCount + col, it.value()));
            }
        }

        // INSERT CONSTRAINT IDENTITY
        for(int i = 0; i < constraintCount; i++) {
            inserts.push_back(T(3* (mVertexCount + i)    ,                  constraints[i], 1));
            inserts.push_back(T(3* (mVertexCount + i) + 1, mVertexCount   + constraints[i], 1));
            inserts.push_back(T(3* (mVertexCount + i) + 2, mVertexCount*2 + constraints[i], 1));
        }

        matrixA.setFromTriplets(inserts.begin(), inserts.end());


        // INSERT LAPLACE OPERATOR THREE TIMES
        // INSERT CONDITION VECTOR THREE TIMES

        mTransposeMatrixA = matrixA.transpose();
        SimplicialLDLT<SparseMatrix<float>> &solver = *mSolver;
        solver.compute(mTransposeMatrixA * matrixA);

        if(solver.info()!= Eigen::Success) {
            std::cout << " FAILED TO COMPUTE " << std::endl;
        }

        // SOLVE USING NORMAL EQUATION
        //mSolver->compute(mTransposeMatrixA * matrixA);
    };

    void SetConstraintPositions(std::vector<glm::vec3> &constraintPositions) {
        mConstraintPositions = constraintPositions;
    }

    void Update(GLFWwindow *window, MouseInput &mouse, float deltaTime) {
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            if(!wasPressedSpace) {
                // COMPUTE
                std::list<unsigned int> constraintIds = *mMesh->GetSelectedVertices();
                std::vector<unsigned int> constVec;
                constVec.reserve(constraintIds.size());

                for(std::list<unsigned int>::iterator it = constraintIds.begin(); it != constraintIds.end(); it++) {
                    constVec.push_back(*it);
                }

                SetConstraints(constVec);
                std::cout << " CONSTRAINT IDS WHERE SET TO " << mConstIds.size() << std::endl;
            }
            wasPressedSpace = true;
        } else {
            wasPressedSpace = false;
        }

        if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
            if(!wasPressedL) {
                if (!liveCompute) {
                    // COMPUTE
                    std::cout << " SET POSITIONS AND COMPUTE " << std::endl;
                    std::vector<glm::vec3> constPositions;

                    for (int i = 0; i < mConstIds.size(); i++) {
                        constPositions.push_back(mMesh->mMesh->mVertices[mConstIds[i]].Position);
                    }
                    SetConstraintPositions(constPositions);
                }
                //ComputeModifier();
                liveCompute = !liveCompute;
                mMesh->ClearSelections();
            }

            wasPressedL = true;
        } else {
            wasPressedL = false;
        }

        if(liveCompute) {
            ComputeModifier();
        }
    }

    ~LaplaceMeshModifier() {
        //delete mSolver;
    }
private:
    bool liveCompute = false;
    bool wasPressedSpace = false;
    bool wasPressedL = false;
    int mVertexCount;
    MeshMatrix mMeshMatrix;
    Eigen::Matrix<float, Eigen::Dynamic, 3> mLaplaceCoords;
    Eigen::SparseMatrix<float> mTransposeMatrixA;
    Eigen::SparseMatrix<float> mLaplaceOperator;

    std::vector<unsigned int> mConstIds;
    std::vector<glm::vec3> mConstraintPositions;

    SimplicialLDLT<SparseMatrix<float>> *mSolver;

    // TAKE CARE THE RESULT IS A Nx3 MATRIX
    // WE ARE LATER USING A N*3 VECTOR
    void ComputeLaplaceOperator() {
        typedef Eigen::Triplet<float> T;

        std::vector<T> tripletList;
        tripletList.reserve(mVertexCount * 3);

        auto &adjMatrix = mMeshMatrix.AdjacencyMatrix;
        auto &valence = mMeshMatrix.Valence;
        mLaplaceOperator.resize(mVertexCount, mVertexCount);
        for(int col = 0; col < mVertexCount; col++) {
            // USING THE LAPLACE DEFINITION FROM THE PAPER
            tripletList.push_back(T(col, col, 1));

            for(SparseMatrix<unsigned char>::InnerIterator it(adjMatrix, col); it; ++it) {
                if(col != it.row()) {
                    tripletList.push_back(T(it.row(), col, -1.0f / valence[it.row()]));
                }
            }
        }
        mLaplaceOperator.setFromTriplets(tripletList.begin(), tripletList.end());

       // std::cout << mLaplaceOperator << std::endl;
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
};
#endif //LAPLACEMESHANIMATOR_LAPLACEMESHMODIFIER_H
