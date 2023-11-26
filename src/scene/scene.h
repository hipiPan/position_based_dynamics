#pragma once

class Renderer;
class SimulationModel;

class Scene
{
public:

    Scene() = default;

    virtual ~Scene() = default;

    virtual SimulationModel* get_simulation_model() { return nullptr; };

    virtual void draw(Renderer* renderer) {}
};