#pragma once
#include <glm/glm.hpp>

bool solve_distance_constraint(
    const glm::vec3& p0, float inv_mass0,
    const glm::vec3& p1, float inv_mass1,
    float rest_length,
    float stiffness,
    glm::vec3& corr0, glm::vec3& corr1);