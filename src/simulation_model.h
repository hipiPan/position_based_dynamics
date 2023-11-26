#pragma once
#include <vector>
#include <glm/glm.hpp>

class ParticleData;
class TriangleModel;
class Constraint;

class SimulationModel
{
public:
    SimulationModel();

    virtual ~SimulationModel();

    TriangleModel* add_triangle_model(uint32_t index_count, uint16_t* indices, uint32_t vertex_count, float* positions, float* normals, float* uvs);

    TriangleModel* add_regular_triangle_model(int width, int height, const glm::vec3& translation, const glm::vec3& rotation, const glm::vec2& scale);

    void add_distance_constraint(int p0, int p1, float stiffness);

    ParticleData* get_particles();

    std::vector<uint32_t>& get_particle_offsets() { return _particle_offsets; }

    std::vector<TriangleModel*>& get_triangle_models() { return _triangle_models; }

    std::vector<Constraint*>& get_constraints() { return _constraints; }

private:
    ParticleData* _particles;
    std::vector<uint32_t> _particle_offsets;
    std::vector<TriangleModel*> _triangle_models;
    std::vector<Constraint*> _constraints;
};