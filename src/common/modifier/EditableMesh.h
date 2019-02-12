#ifndef EditableMesh_hpp
#define EditableMesh_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "../ShaderManager.h"
#include "../drawables/Mesh.h"
#include "../RenderIDObject.h"
#include "../Util.h"

using namespace std;

const glm::vec4 ACTIVE_MESH_COLOR(0.9, 0.9, 0.9, 1);
const glm::vec4 SELECTED_MESH_COLOR(1,1,0,1);
const glm::vec4 NORMAL_MESH_COLOR(0.5, 0.5, 0.5, 1);

const glm::vec4 SELECTED_VERTEX_COLOR(1,0,0,1);
const glm::vec4 UNSELECTED_VERTEX_COLOR(0.4, 0.4, 0.4, 1);

enum DrawMode {
    MESH_WIREFRAME_MODE,
    MESH_VERTEX_MODE,
    MESH_NORMAL_MODE
};

class EditableMesh : public RenderIDObject, public RenderObject {
public:
    EditableMesh(Mesh &mesh, unsigned int id = 0);

    void SetDrawMode(DrawMode mode) { mode = drawMode; };

    void Draw(Camera &camera) override;
    void DrawWireframe(Camera &camera);
    void DrawVertexHandles(Camera &camera);
    void DrawVertexHandleIds(Camera &camera);
    void DrawID(Camera &camera) override;
    void DrawID(Camera &camera, unsigned int id);

    void ToggleVertexSelection(unsigned int vertexID);
    void ClearSelections();
    //void HighlightVertex(unsigned int vertexID);
    
    std::list<unsigned int> GetSelectedVertices() {
        return mHighlightedVertices;
    }

    void SetActive(bool b) {
        mActive = b;
        mSelected = mSelected && mActive;

        if(!mActive) {
            ClearSelections();
        }

        UpdateColor();
    }

    void SetSelected(bool b) {
        mSelected = b;
        UpdateColor();
    }


    Mesh *mMesh;
private:
    float mHandleSize;
    glm::vec4 mColor;
    DrawMode drawMode;
    bool mActive;
    bool mSelected;
    std::vector<glm::vec4> mColorCodes;
    std::vector<glm::vec4> mHandleColors;
    GLuint mColorCodeBuffer, mHandleColorBuffer;

    std::list<unsigned int> mHighlightedVertices;

    void UpdateColor() {
        if(mActive) {
            if(mSelected) {
                mColor = SELECTED_MESH_COLOR;
            } else {
                mColor = ACTIVE_MESH_COLOR;
            }
        } else {
            mColor = NORMAL_MESH_COLOR;
        }
    };

    void UpdateHandleColors() {
        for(auto &color : mHandleColors) {
            color = UNSELECTED_VERTEX_COLOR;
        }

        for(auto &id : mHighlightedVertices) {
            mHandleColors[id] = SELECTED_VERTEX_COLOR;
        }

        glBindBuffer(GL_ARRAY_BUFFER, mHandleColorBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * mHandleColors.size(), &mHandleColors[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*) 0);
    }

    bool ContainsID(unsigned int compareId, std::list<unsigned int> &ids) {
        for(auto &id : ids) {
            if(compareId == id) {
                return true;
            }
        }

        return false;
    }
};

#endif /* EditableMesh_hpp */
