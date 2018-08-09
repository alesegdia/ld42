#include "menu.h"

#include "../ld42.h"


MenuScreen::MenuScreen(LD42 *game)
{
    m_game = game;
}

void MenuScreen::show()
{
    m_game->assets.sample_anim.reset(m_animData);
}

void MenuScreen::hide()
{

}

void MenuScreen::render()
{
    aether::graphics::clear(1, 0, 0);
    m_game->assets.sample_texture.draw(0, 0);
    auto frame = m_game->assets.sample_sheet.getFrame(2);
    frame.draw(100, 100);
    m_game->assets.sample_anim.updateData(m_animData);
    m_animData.render(200, 200);
}


void MenuScreen::update(uint64_t delta)
{
    m_animData.timer += delta;
}
