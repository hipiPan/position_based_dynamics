#include "simulation.h"
#include "simulation_model.h"
#include "particle_data.h"
#include "integration.h"
#include "constraint.h"

Simulation::Simulation()
{
    _sub_steps = 1;
    _step_time = 0.5f;
    _time = 0.0f;
}

Simulation::~Simulation()
{
}

void Simulation::step(SimulationModel* model)
{
    ParticleData* particles = model->get_particles();

    for (int j = 0; j < particles->x.size(); ++j)
    {
        particles->last_x[j] = particles->old_x[j];
        particles->old_x[j] = particles->x[j];
        semi_implicit_euler(_step_time, particles->masses[j], particles->a[j], particles->x[j], particles->v[j]);
    }

    position_constraint_projection(model);

    for (int j = 0; j < particles->x.size(); ++j)
    {
        velocity_update_second_order(_step_time, particles->masses[j], particles->x[j], particles->old_x[j], particles->last_x[j], particles->v[j]);
    }

    // TODO: Collision Detection

    velocity_constraint_projection(model);

    _time += _step_time;
}

void Simulation::position_constraint_projection(SimulationModel* model)
{
    std::vector<Constraint*>& constraints = model->get_constraints();

    int iterations = 0;
    int max_iterations = 2;
    while (iterations < max_iterations)
    {
        for (auto constraint : constraints)
        {
            constraint->update_constraint();
            constraint->solve_position_constraint(iterations);
        }
        iterations++;
    }
}

void Simulation::velocity_constraint_projection(SimulationModel* model)
{
    std::vector<Constraint*>& constraints = model->get_constraints();

    int iterations = 0;
    int max_iterations = 2;
    while (iterations < max_iterations)
    {
        for (auto constraint : constraints)
        {
            constraint->update_constraint();
            constraint->solve_velocity_constraint(iterations);
        }
        iterations++;
    }
}