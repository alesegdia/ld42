
#include "ld42.h"

LD42::LD42(int w, int h)
    : aether::core::Application(w, h),
      menuScreen(this),
      gameplayScreen(this),
      endScreen(this)
{

}

int LD42::ready(int argc, char **argv)
{
    assets.load();
    setScreen(&menuScreen);
    //assets.theme.playmode(aether::audio::PlayMode::Loop);
    assets.theme.play();
    return 0;
}

void LD42::update(uint64_t delta)
{
    aether::core::Application::update(delta);
    if( aether::core::is_key_down(aether::core::KeyCode::Escape) )
    {
        close();
    }
}

void LD42::dispose()
{
    assets.cleanup();
}
