#pragma once
#include <vector>
#include <glm/glm.hpp>

class ParticleData;
class TriangleModel;

class SimulationModel
{
public:
    SimulationModel();

    virtual ~SimulationModel();

    TriangleModel* add_triangle_model(uint32_t index_count, uint16_t* indices, uint32_t vertex_count, float* positions, float* normals, float* uvs);

    TriangleModel* add_regular_triangle_model(int width, int height, const glm::vec3& translation, const glm::vec3& rotation, const glm::vec2& scale);

    ParticleData* get_particles();

private:
    ParticleData* _particles;
    std::vector<TriangleModel*> _triangle_models;
};