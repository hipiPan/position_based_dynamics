#include "scene_loader.h"
#include "cloth_scene.h"

Scene* load_scene(SceneType type)
{
    Scene* scene = nullptr;
    switch (type)
    {
        case SceneType::CLOTH:
            scene = new ClothScene();
            break;
    }
    return scene;
}

void destroy_scene(Scene* scene)
{
    if (scene)
        delete scene;
}