#pragma once

#include <aether/aether.h>

class LD42;

class EndScreen : public aether::core::IScreen
{
public:
    EndScreen(LD42* game);

    void show() final;
    void hide() final;
    void render() final;
    void update(uint64_t delta) final;

private:
    LD42* m_game;

};
