#pragma once
#include <glm/glm.hpp>
#include "triangle_mesh.h"

class TriangleModel : public TriangleMesh
{
public:
    TriangleModel(uint32_t index_count, uint16_t* indices, uint32_t vertex_count, float* positions, float* normals, float* uvs);

    ~TriangleModel();

    float restitution_coeff;
    float friction_coeff;
};