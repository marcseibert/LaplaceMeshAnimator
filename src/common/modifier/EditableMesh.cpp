#include "EditableMesh.h"

#include "Mesh.h"
#include <iostream>

EditableMesh::EditableMesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    
    setupMesh();
}


void EditableMesh::setupMesh() {
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    
    glBindVertexArray(0);
    
    glGenVertexArrays(1, &hVAO);
    glGenBuffers(1, &hEBO);
    
    glBindVertexArray(hVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, sizeof(Vertex), (void*) 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, highlightedVertices.size() * sizeof(unsigned int), &highlihgtedVertices[0], GL_STATIC_DRAW);
}

void Mesh::Draw(Shader &shader) {
    
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = textures[i].type;
        if(name == "texture_diffuse"){
            //cout << " DRAWING TEXTURE " << diffuseNr << std::endl;
            number = std::to_string(diffuseNr++);
        }
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);
        
        glUniform1i(glGetUniformLocation(shader.ID, ("material." + name + number).c_str()), i);
        std::cout << ("material." + name + number).c_str() << " " << glGetUniformLocation(shader.ID, ("material." + name + number).c_str()) << " TEX POS" << std::endl;
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    
    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    glActiveTexture(GL_TEXTURE0);
    // DRAW DOTS IF NEEDED
    
    if(drawMode == MESH_VERTEX_MODE) {
        glBindVertexArray(hVAO);
        glDrawElements(GL_POINTS, highlightedVertices.size(), GL_UNSIGNED_INT, 0);
    }
}

