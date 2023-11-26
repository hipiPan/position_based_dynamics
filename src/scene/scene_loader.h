#pragma once

class Scene;

enum class SceneType
{
    CLOTH,
};

Scene* load_scene(SceneType type);

void destroy_scene(Scene* scene);