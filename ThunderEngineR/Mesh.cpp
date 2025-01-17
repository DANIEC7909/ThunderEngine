#include "Mesh.h"
#include <iostream>
#include "TEngine.h"

void Mesh::Load(const char* ModelPath,const char* mtl_path) {
  
    if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &err, ModelPath, mtl_path,true)) {
        std::cerr << err << std::endl;
        exit(EXIT_FAILURE);
    }

   
    for (int i = 0; i < shapes.size(); i++) {
        tinyobj::shape_t& shape = shapes[i];
        tinyobj::mesh_t& mesh = shape.mesh;
        for (int j = 0; j < mesh.indices.size(); j++) {
            tinyobj::index_t i = mesh.indices[j];
            glm::vec3 position = {
                attributes.vertices[i.vertex_index * 3],
                attributes.vertices[i.vertex_index * 3 + 1],
                attributes.vertices[i.vertex_index * 3 + 2]
            };
            glm::vec3 normal = {
                attributes.vertices[i.normal_index * 3],
                attributes.vertices[i.normal_index * 3 + 1],
                attributes.vertices[i.normal_index * 3 + 2]
            };
            glm::vec2 texCoord = {
                attributes.vertices[i.texcoord_index * 2],
                attributes.vertices[i.texcoord_index * 2 + 1],
            };
            Vertex vert = { position, normal, texCoord };
            vertices.push_back(vert);
        }
    }
}
void Mesh::SetupMesh()
{
  
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
   
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
}

void Mesh::Draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

}
