//
//  EditableMesh.hpp
//  Globe
//
//  Created by Marc Seibert on 08.02.19.
//  Copyright © 2019 Marc Seibert. All rights reserved.
//

#ifndef EditableMesh_hpp
#define EditableMesh_hpp

#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "ShaderManager.hpp"
#include "shader.hpp"
#include "../Mesh.hpp"

using namespace std;

enum DrawMode {
    MESH_WIREFRAME_MODE,
    MESH_VERTEX_MODE,
    MESH_NORMAL_MODE
};

class EditableMesh {
public:
    EditableMesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void Draw(Shader &shader);
    void SetDrawMode(DrawMode mode) { drawMode = drawMode; };
    void ClearHighlights();
    void HighlightVertex();
    
    vector<Vertex> GetHighlightedVertices();
    
    
private:
    
    void setupMesh();
    
    DrawMode drawMode;
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    
    vector<unsigned int> highlightedVertices;
    
    // MESH MODIFICATIONS
    
    /* OPENGL OBJECT HANDLES */
    unsigned int VAO, VBO, EBO, hVAO,hEBO;
};

#endif /* EditableMesh_hpp */
