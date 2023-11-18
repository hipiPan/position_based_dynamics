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