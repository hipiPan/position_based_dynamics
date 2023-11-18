#pragma once
#include <vector>
#include <glm/glm.hpp>

class ParticleData
{
public:
    ParticleData();

    ~ParticleData();

    void clear();

    void add_vertex(const glm::vec3& vertex);

    // Mass
    std::vector<float> masses;
    std::vector<float> inv_masses;

    // Dynamic state
    std::vector<glm::vec3> x0;
    std::vector<glm::vec3> x;
    std::vector<glm::vec3> v;
    std::vector<glm::vec3> a;
    std::vector<glm::vec3> old_x;
    std::vector<glm::vec3> last_x;
};