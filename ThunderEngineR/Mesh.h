
#pragma once
#include "tiny_obj_loader.h"
#include "Asset.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
class Vertex {
public:
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};
class Mesh : Asset
{
public:
    void Load(const char* ModelPath, const char* mtl_path);
    void SetupMesh();
    void Draw();
public:
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::vector<Vertex> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    std::string  err;
    unsigned int VAO;
    unsigned int  VBO;
};

