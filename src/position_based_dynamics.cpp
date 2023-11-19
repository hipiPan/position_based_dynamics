#include "position_based_dynamics.h"

bool solve_distance_constraint(
    const glm::vec3& p0, float inv_mass0,
    const glm::vec3& p1, float inv_mass1,
    float rest_length,
    float stiffness,
    glm::vec3& corr0, glm::vec3& corr1)
{
    float w_sum = inv_mass0 + inv_mass1;
    if (w_sum == 0.0f)
        return false;

    glm::vec3 n = p1 - p0;
    float d = glm::length(n);
    n = glm::normalize(n);

    glm::vec3 corr = stiffness * n * (d - rest_length) / w_sum;
    corr0 = inv_mass0 * corr;
    corr1 = inv_mass1 * corr;

    return true;
}