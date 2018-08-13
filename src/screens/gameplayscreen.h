#pragma once

#include <aether/aether.h>

#include "../core/scene.h"

class LD42;

class GameplayScreen : public aether::core::IScreen
{
public:
    GameplayScreen(LD42* game);

    void show() final;
    void hide() final;
    void render() final;
    void update(uint64_t delta) final;

    Scene::Status status()
    {
        return m_scene->status();
    }

    std::shared_ptr<Scene> scene()
    {
        return m_scene;
    }

private:
    LD42* m_game;
    std::shared_ptr<Scene> m_scene;

};
