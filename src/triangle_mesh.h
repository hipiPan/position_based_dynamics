#pragma once
#include <array>
#include <vector>
#include <glm/glm.hpp>

class TriangleMesh
{
public:
    struct Edge
    {
        std::array<unsigned int, 2> face;
        std::array<unsigned int, 2> vert;
    };

    TriangleMesh(uint32_t index_count, uint16_t* indices, uint32_t vertex_count , float* positions, float* normals, float* uvs);

    virtual ~TriangleMesh();

    void update_normals();

    void build_neighbors();

    uint32_t index_count;
    uint16_t* indices;
    uint32_t vertex_count;
    uint32_t face_count;
    float* positions;
    float* normals;
    float* uvs;
    std::vector<Edge> edges;
    std::vector<std::vector<uint32_t>> faces_edges;
    std::vector<std::vector<uint32_t>> vertices_edges;
    std::vector<std::vector<uint32_t>> vertices_faces;
};