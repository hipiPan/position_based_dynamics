#include "cloth_scene.h"
#include "../simulation.h"
#include "../simulation_model.h"
#include "../particle_data.h"
#include "../triangle_model.h"
#include "../renderer.h"

ClothScene::ClothScene()
{
    _simulation_model = new SimulationModel();

    int cols = 3;
    int rows = 3;
    glm::vec3 translation = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 3.1415926f / 2.0f, 0.0f);
    glm::vec2 scale = glm::vec2(4.0f, 4.0f);
    _simulation_model->add_regular_triangle_model(cols, rows, translation, rotation, scale);

    // Make points static
    ParticleData* particles = _simulation_model->get_particles();
    particles->set_mass(0, 0.0f);
    particles->set_mass(rows - 1, 0.0f);

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
    ParticleData* particles = _simulation_model->get_particles();
    for (int i = 0; i < particles->x.size(); ++i)
    {
        renderer->draw_point(particles->x[i], glm::vec4(0.61f, 0.97f, 0.05f, 1.0f));
    }

    TriangleModel* tm = _simulation_model->get_triangle_models()[0];
    uint32_t particle_offset = _simulation_model->get_particle_offsets()[0];
    for (int i = 0; i < tm->edges.size(); ++i)
    {
        int p0 = tm->edges[i].vert[0] + particle_offset;
        int p1 = tm->edges[i].vert[1] + particle_offset;
        renderer->draw_line(particles->x[p0], particles->x[p1], glm::vec4(0.98f, 0.549f, 0.2078f, 1.0f));
    }
}