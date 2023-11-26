#include "cloth_scene.h"
#include "../simulation.h"
#include "../simulation_model.h"
#include "../particle_data.h"
#include "../triangle_model.h"
#include "../renderer.h"

ClothScene::ClothScene()
{
    _simulation_model = new SimulationModel();

    int cols = 2;
    int rows = 2;
    glm::vec3 translation = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec2 scale = glm::vec2(2.0f, 2.0f);
    _simulation_model->add_regular_triangle_model(cols, rows, translation, rotation, scale);

    // Make points static
    ParticleData* particles = _simulation_model->get_particles();
    particles->masses[0] = 0.0f;
    particles->masses[rows - 1] = 0.0f;

    // Cloth constraints
    float cloth_stiffness = 1.0f;
    TriangleModel* tm = _simulation_model->get_triangle_models()[0];
    uint32_t particle_offset = _simulation_model->get_particle_offsets()[0];
    for (int i = 0; i < tm->edges.size(); ++i)
    {
        int p0 = tm->edges[i].vert[0] + particle_offset;
        int p1 = tm->edges[i].vert[1] + particle_offset;
        _simulation_model->add_distance_constraint(p0, p1, cloth_stiffness);
    }
}

ClothScene::~ClothScene()
{
    delete _simulation_model;
}

SimulationModel* ClothScene::get_simulation_model()
{
    return _simulation_model;
}

void ClothScene::draw(Renderer* renderer)
{

}