#pragma once
#include <glm/glm.hpp>

void semi_implicit_euler(
    const float h,
    const float mass,
    const glm::vec3& acceleration,
    glm::vec3& position,
    glm::vec3& velocity);

void velocity_update_first_order(
    const float h,
    const float mass,
    const glm::vec3& position,
    const glm::vec3& old_position,
    glm::vec3& velocity
    );

void velocity_update_second_order(
    const float h,
    const float mass,
    const glm::vec3& position,
    const glm::vec3& old_position,
    const glm::vec3& last_position,
    glm::vec3& velocity
);