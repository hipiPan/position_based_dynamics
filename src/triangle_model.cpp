#include "triangle_model.h"

TriangleModel::TriangleModel(uint32_t index_count, uint16_t* indices, uint32_t vertex_count, float* positions, float* normals, float* uvs)
: TriangleMesh(index_count, indices, vertex_count, positions, normals, uvs)
{
    build_neighbors();
    restitution_coeff = 0.6f;
    friction_coeff = 0.2f;
}

TriangleModel::~TriangleModel()
{
}