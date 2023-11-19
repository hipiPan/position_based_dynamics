#include "simulation_model.h"
#include "constraint.h"
#include "particle_data.h"
#include "triangle_mesh.h"
#include "triangle_model.h"
#include <math/transform_util.h>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

SimulationModel::SimulationModel()
{
    _particles = new ParticleData();
}

SimulationModel::~SimulationModel()
{
    delete _particles;
    _particle_offsets.clear();

    for (auto tm : _triangle_models)
    {
        delete tm;
    }
    _triangle_models.clear();

    for (auto constraint : _constraints)
    {
        delete constraint;
    }
    _constraints.clear();
}

TriangleModel* SimulationModel::add_triangle_model(uint32_t index_count, uint16_t* indices, uint32_t vertex_count, float* positions, float* normals, float* uvs)
{
    _particle_offsets.push_back(_particles->x.size());
    for (int i = 0; i < vertex_count; ++i)
    {
        _particles->add_vertex(glm::vec3(positions[i*3], positions[i*3+1], positions[i*3+2]));
    }

    TriangleModel* tm = new TriangleModel(index_count, indices, vertex_count, positions, normals, uvs);
    _triangle_models.push_back(tm);
    return tm;
}

TriangleModel* SimulationModel::add_regular_triangle_model(int width, int height, const glm::vec3& translation, const glm::vec3& rotation, const glm::vec2& scale)
{
    glm::mat4 rotation_matrix = glm::eulerAngleYXZ(rotation.x, rotation.y, rotation.z);

    float dx = scale[0] / float(width);
    float dy = scale[1] / float(height);

    uint32_t vertex_count = width * height;
    float* positions = new float[vertex_count * 3];
    float* uvs = new float[vertex_count * 2];
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            float x = dx * (float)j;
            float y = dy * (float)i;

            glm::vec3 point = transform_point(glm::vec3(x, y, 0.0f), rotation_matrix) + translation;
            int idx = i * width + j;
            positions[idx * 3] = point.x;
            positions[idx * 3 + 1] = point.y;
            positions[idx * 3 + 2] = point.z;

            uvs[idx * 2] = x / scale[0];
            uvs[idx * 2 + 1] = y / scale[1];
        }
    }

    uint32_t index_count = 6 * (width - 1) * (height - 1);
    uint16_t* indices = new uint16_t[index_count];
    int idx = 0;
    for (int i = 0; i < height - 1; ++i)
    {
        for (int j = 0; j < width - 1; ++j)
        {
            int helper = i % 2 == j % 2 ? 1 : 0;

            indices[idx] = i * width + j;
            indices[idx + 1] = i * width + j + 1;
            indices[idx + 2] = (i + 1) * width + j + helper;
            idx += 3;

            indices[idx] = (i + 1) * width + j + 1;
            indices[idx + 1] = (i + 1) * width + j;
            indices[idx + 2] = i * width + j + 1 - helper;
            idx += 3;
        }
    }

    TriangleModel* tm = add_triangle_model(index_count, indices, vertex_count, positions, nullptr, uvs);
    tm->update_normals();

    return tm;
}

void SimulationModel::add_distance_constraint(int p0, int p1, float stiffness)
{
    DistanceConstraint* c = new DistanceConstraint(this, p1, p1, stiffness);
    _constraints.push_back(c);
}

ParticleData* SimulationModel::get_particles()
{
    return _particles;
}