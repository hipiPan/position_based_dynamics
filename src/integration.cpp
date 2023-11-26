#include "integration.h"

void semi_implicit_euler(
    const float h,
    const float mass,
    const glm::vec3& acceleration,
    glm::vec3& position,
    glm::vec3& velocity)
{
    if (mass != 0.0)
    {
        velocity += acceleration * h;
        position += velocity * h;
    }
}

void velocity_update_first_order(
    const float h,
    const float mass,
    const glm::vec3& position,
    const glm::vec3& old_position,
    glm::vec3& velocity
)
{
    if (mass == 0.0f)
        return;

    velocity = glm::vec3(1.0f / h) * (position - old_position);
}

void velocity_update_second_order(
    const float h,
    const float mass,
    const glm::vec3& position,
    const glm::vec3& old_position,
    const glm::vec3& last_position,
    glm::vec3& velocity
)
{
    if (mass == 0.0f)
        return;

    velocity = glm::vec3(1.0f / h) * (1.5f * position - 2.0f * old_position + 0.5f * last_position);
}