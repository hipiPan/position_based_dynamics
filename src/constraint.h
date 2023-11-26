#pragma once
#include <vector>

class SimulationModel;

class Constraint
{
public:
    Constraint(SimulationModel* model) { _model = model; }

    virtual ~Constraint() {}

    virtual bool update_constraint() { return true; }

    virtual bool solve_position_constraint(int iter) { return true; }

    virtual bool solve_velocity_constraint(int iter) { return true; }

protected:
    SimulationModel* _model;
    std::vector<uint32_t> _bodies;
};

class DistanceConstraint : public Constraint
{
public:
    DistanceConstraint(SimulationModel* model, int p0, int p1, float stiffness);

    virtual bool solve_position_constraint(int iter);

protected:
    float _stiffness;
    float _rest_length;
};