#pragma once

class SimulationModel;

class Simulation
{
public:
    Simulation();

    ~Simulation();

    void step(SimulationModel* model);

protected:
    void position_constraint_projection(SimulationModel* model);

    void velocity_constraint_projection(SimulationModel* model);

private:
    int _sub_steps;
    float _step_time;
    float _time;
};