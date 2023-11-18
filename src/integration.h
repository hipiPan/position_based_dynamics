#pragma once
#include <glm/glm.hpp>

void semi_implicit_euler(
    const float h,
    const float mass,
    const glm::vec3& acceleration,
    glm::vec3& position,
    glm::vec3& velocity);