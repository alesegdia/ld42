#pragma once

#include <aether/aether.h>

class LD42;

class MenuScreen : public aether::core::IScreen
{
public:
    MenuScreen(LD42* game);

    void show() final;
    void hide() final;
    void render() final;
    void update(uint64_t delta) final;

private:
    LD42* m_game;
    aether::graphics::AnimationData m_animData;

};
