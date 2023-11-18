#pragma once

class SimulationModel;

class Simulation
{
public:
    Simulation();

    ~Simulation();

    void step(SimulationModel* model);

private:
    int _sub_steps;
    float _step_time;
    float _time;
};