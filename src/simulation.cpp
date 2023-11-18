#include "simulation.h"
#include "simulation_model.h"
#include "particle_data.h"
#include "integration.h"

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

    float sub_step_time = _step_time / _sub_steps;
    for (int i = 0; i < _sub_steps; ++i)
    {
        // Particle model
        for (int j = 0; j < particles->x.size(); ++j)
        {
            particles->last_x[j] = particles->old_x[j];
            particles->old_x[j] = particles->x[j];
            semi_implicit_euler(sub_step_time, particles->masses[j], particles->a[j], particles->x[j], particles->v[j]);
        }
    }



    _time += _step_time;
}