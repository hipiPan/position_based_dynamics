#include "triangle_mesh.h"

TriangleMesh::TriangleMesh(uint32_t index_count, uint16_t* indices, uint32_t vertex_count, float* positions, float* normals, float* uvs)
{
    this->index_count = index_count;
    this->indices = indices;
    this->vertex_count = vertex_count;
    this->face_count = index_count / 3;
    this->positions = positions;
    this->normals = normals;
    this->uvs = uvs;
}

TriangleMesh::~TriangleMesh()
{
    if (indices)
        delete[] indices;

    if (positions)
        delete[] positions;

    if (normals)
        delete[] normals;

    if (uvs)
        delete[] uvs;

    if (edges)
        delete[] edges;
}

void TriangleMesh::build_neighbors()
{

}

void TriangleMesh::update_normals()
{
    if (!normals)
        normals = new float[vertex_count * 3];

    memset(normals, 0, sizeof(float) * vertex_count * 3);

    for (int i = 0; i < face_count; ++i)
    {
        uint16_t index0 = indices[i * 3];
        uint16_t index1 = indices[i * 3 + 1];
        uint16_t index2 = indices[i * 3 + 2];
        glm::vec3 v0 = glm::vec3(positions[index0 * 3], positions[index0 * 3 + 1], positions[index0 * 3 + 2]);
        glm::vec3 v1 = glm::vec3(positions[index1 * 3], positions[index1 * 3 + 1], positions[index1 * 3 + 2]);
        glm::vec3 v2 = glm::vec3(positions[index2 * 3], positions[index2 * 3 + 1], positions[index2 * 3 + 2]);

        glm::vec3 e0 = v1 - v0;
        glm::vec3 e1 = v2 - v0;
        glm::vec3 n = glm::normalize(glm::cross(e0, e1));

        normals[index0 * 3] += n.x;
        normals[index0 * 3 + 1] += n.y;
        normals[index0 * 3 + 2] += n.z;
        normals[index1 * 3] += n.x;
        normals[index1 * 3 + 1] += n.y;
        normals[index1 * 3 + 2] += n.z;
        normals[index2 * 3] += n.x;
        normals[index2 * 3 + 1] += n.y;
        normals[index2 * 3 + 2] += n.z;
    }
}