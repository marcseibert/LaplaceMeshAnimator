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
            //std::cout << "POSITION " << it->x << " " << it->y << " " << it->z << std::endl;
            b(constraintIterCount++) = it->x *constraintWeight; // vertices[*it].Position.x;
            b(constraintIterCount++) = it->y *constraintWeight; // vertices[*it].Position.y;
            b(constraintIterCount++) = it->z *constraintWeight; // vertices[*it].Position.z;

        }
        /*
        for(int i = 0; i < mVertexCount; i++) {
            //std::cout << "b: " << b(i) << " " << b(mVertexCount + i) << " " <<b(2*mVertexCount + i) << std::endl;
        }
        //std::cout << "---- CONSTRAINTS ------" << std::endl;
        for(int i = 0; i < constraintCount; i++) {
            std::cout << "b: " << b(mVertexCount*3 +i*3) << " " << b(mVertexCount*3 + i*3 +1) << " " <<b(mVertexCount*3 + i*3 +2) << std::endl;
        } */

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
            //std::cout << "i: " << x(i) << " " << x(mVertexCount + i) << " " <<x(2*mVertexCount + i) << std::endl;
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

            inserts.push_back(T(3* (mVertexCount + i)    ,                  constraints[i], constraintWeight));
            inserts.push_back(T(3* (mVertexCount + i) + 1, mVertexCount  + constraints[i], constraintWeight));
            inserts.push_back(T(3* (mVertexCount + i) + 2, mVertexCount*2 + constraints[i], constraintWeight));
        }

        //inserts.push_back(T(3* (mVertexCount + constraintCount - 1)    ,                  constraints[constraintCount - 1], 100* constraintWeight));
        //inserts.push_back(T(3* (mVertexCount + constraintCount - 1) + 1, mVertexCount  + constraints[constraintCount - 1], 100 *constraintWeight));
        //inserts.push_back(T(3* (mVertexCount + constraintCount - 1) + 2, mVertexCount*2 + constraints[constraintCount - 1], 100 *constraintWeight));

        matrixA.setFromTriplets(inserts.begin(), inserts.end());
        //std::cout << matrixA << std::endl;
        /*
        std::cout << std::endl << " CONSTRAINTS" << std::endl;
        for(int i = 0; i < constraints.size();i++) {
            std::cout << i << " " << constraints[i] << std::endl;
        } std::cout << std::endl;
        */
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

    void Update(GLFWwindow *window, Renderer &renderer, Camera &camera, MouseInput &mouse, float deltaTime) {
        if(!mMesh || !mEnabled) {
            return;
        }

        if(mouse.IsNewPressed(MOUSE_BUTTON_LEFT) && mMesh->IsSelected() &&!isDragging) {
            unsigned int clickedVertex = mMesh->CheckVertexIntersection(renderer, camera, mouse.GetPosition());

            if(clickedVertex != 0) {
                clickedVertex--;
                std::cout << "LAPLACE: CLICKED ON VERTEX " << clickedVertex << std::endl;
                //mMesh->ToggleVertexSelection(clickedVertex);
                ComputeLaplaceCoords();
                mMesh->ClearSelection();
                glm::vec4 blueColor(0,0,1,1);
                auto neighborhood = mMeshMatrix.GetVerticesInRange(clickedVertex, 5);
                auto neighborhoodTwo = mMeshMatrix.GetVerticesInRange(clickedVertex, 3);

                std::cout << " SELECTING " << neighborhood.size() << " vertices" << std::endl;
                //mMesh->SelectAll();

                //mMesh->UnselectVertexSet(neighborhood, blueColor);
                mMesh->SetSelectionSet(neighborhood, blueColor);
                mMesh->UnselectVertexSet(neighborhoodTwo, UNSELECTED_VERTEX_COLOR);

                mMesh->SetVertexSelection(clickedVertex, true, glm::vec4(1,1,0,1));

                mDragOrigin = glm::vec3(mouse.GetPosition(), 0);
                selectedVertex = clickedVertex;

                // SET CONSTRAINT IDS
                std::list<unsigned int> constraintIds = *mMesh->GetSelectedVertices();
                std::vector<unsigned int> constVec;
                constVec.reserve(constraintIds.size());

                for(std::list<unsigned int>::iterator it = constraintIds.begin(); it != constraintIds.end(); it++) {
                    constVec.push_back(*it);
                }
                SetConstraints(constVec);

                // SET FIXED POSITION SET
                std::vector<glm::vec3> constPositions;

                for (int i = 0; i < mConstIds.size(); i++) {
                    constPositions.push_back(mMesh->mMesh->mVertices[mConstIds[i]].Position);
                    if(mConstIds[i] == clickedVertex) {
                        selectedVertexVectorPosition = i;
                    }
                }
                SetConstraintPositions(constPositions);
                mHandleOriginPosition = mMesh->mMesh->mVertices[selectedVertex].Position;
                //mDeltaDrag = glm::vec3(0);
                isDragging = true;
            }
        }

        if(!mouse.IsPressed(MOUSE_BUTTON_LEFT) && mMesh->IsSelected() && isDragging) {
            isDragging = false;
            std::cout << " RELEASE " << std::endl;
            //mConstraintPositions[selectedVertexVectorPosition].x += 1;
        }

        if(isDragging) {

            glm::vec3 worldSpaceDrag = camera.ScreenToWorldSpace(mDragOrigin - glm::vec3(mouse.GetPosition(), 0));
            std::cout << "drag-x: " << worldSpaceDrag.x << " drag-y: " << worldSpaceDrag.y << std::endl;
            std::cout << "drag-x: " << mDragOrigin.x - mouse.GetPosition().x << " drag-y: " << worldSpaceDrag.y << std::endl;

            // REVERT PREVIOUS CHANGES
            //mDeltaDrag = -mDragVector + worldSpaceDrag;
            //mDragVector = worldSpaceDrag;

            glm::mat4 inverse = glm::inverse(mMesh->mMesh->GetGlobalTransform());
            glm::vec3 newPos = mHandleOriginPosition + glm::vec3(inverse * glm::vec4(worldSpaceDrag, 0));//glm::vec4(mDeltaDrag,0));

            // UPDATE POSTION
            mConstraintPositions[selectedVertexVectorPosition] = newPos;
            //SetConstraintPositions();
            // RECALCULATE MESH
            ComputeModifier();
        }

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
                mMesh->ClearSelection();
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

    void Enable() override {
        mEnabled = true;
    };

    void Disable() override {
        mEnabled = false;
    };

private:

    // SELECTED HANDLE
    float constraintWeight = 100.0f;
    unsigned int selectedVertexVectorPosition; // FOR FASTER VECTOR ACCESS
    unsigned int selectedVertex;
    glm::vec3 mDragOrigin;
    glm::vec3 mHandleOriginPosition;

    bool isDragging = false;
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

            // DIESER OPERATOR LIEGT LINEAR IN DEN RESULTAT VERTICES. D.H. DA DIESER OPERATOR KONSTANT, FUER ALLE
            // MOEGLICHEN VARIATIONEN BLEIBT, MUSS DIES NICHT IN DER ERROR FUNCTION BERUECKSICHTIG WERDEN

            // DIESER TRANSFORM OPERATOR VERSUCHT ISOTROHPISCHE SKALIERUNG UND KLEINE ROTATIONEN ZU VERBESSERN.

        }*/
    }
};
#endif //LAPLACEMESHANIMATOR_LAPLACEMESHMODIFIER_H
