#include "constraint.h"
#include "simulation_model.h"
#include "particle_data.h"
#include "position_based_dynamics.h"

DistanceConstraint::DistanceConstraint(SimulationModel* model, int p0, int p1, float stiffness)
: Constraint(model)
{
    _bodies.resize(2);
    _bodies[0] = p0;
    _bodies[1] = p1;

    ParticleData* particles = _model->get_particles();
    glm::vec3 x0 = particles->x0[p0];
    glm::vec3 x1 = particles->x0[p1];
    _rest_length = glm::length(x1 - x0);

    _stiffness = stiffness;
}

bool DistanceConstraint::solve_position_constraint(int iter)
{
    ParticleData* particles = _model->get_particles();

    glm::vec3 corr0, corr1;
    bool ret = solve_distance_constraint(
        particles->x[_bodies[0]], particles->inv_masses[_bodies[0]],
        particles->x[_bodies[1]], particles->inv_masses[_bodies[1]],
        _rest_length, _stiffness, corr0, corr1);

    if (ret)
    {
        if (particles->inv_masses[_bodies[0]] != 0.0)
            particles->x[_bodies[0]] += corr0;

        if (particles->inv_masses[_bodies[1]] != 0.0)
            particles->x[_bodies[1]] += corr1;
    }

    return ret;
}