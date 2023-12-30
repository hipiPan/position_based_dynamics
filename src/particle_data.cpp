#include "particle_data.h"

ParticleData::ParticleData()
{
}

ParticleData::~ParticleData()
{
    clear();
}

void ParticleData::clear()
{
    masses.clear();
    inv_masses.clear();
    x0.clear();
    x.clear();
    v.clear();
    a.clear();
    old_x.clear();
    last_x.clear();
}

void ParticleData::add_vertex(const glm::vec3& vertex)
{
    x0.push_back(vertex);
    x.push_back(vertex);
    old_x.push_back(vertex);
    last_x.push_back(vertex);
    masses.push_back(1.0);
    inv_masses.push_back(1.0);
    v.push_back(glm::vec3(0.0, 0.0, 0.0));
    a.push_back(glm::vec3(0.0, 0.0, 0.0));
}

void ParticleData::set_mass(int i, float mass)
{
    masses[i] = mass;
    if (mass != 0.0f)
        inv_masses[i] = 1.0f / mass;
    else
        inv_masses[i] = 0.0f;
}