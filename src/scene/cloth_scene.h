#pragma once
#include "scene.h"

class ClothScene : public Scene
{
public:
    ClothScene();

    ~ClothScene();

    virtual SimulationModel* get_simulation_model() override;

    virtual void draw(Renderer* renderer) override;

private:
    SimulationModel* _simulation_model = nullptr;
};