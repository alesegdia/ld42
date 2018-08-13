#include "menu.h"

#include "../ld42.h"


MenuScreen::MenuScreen(LD42 *game)
{
    m_game = game;
}

void MenuScreen::show()
{

}

void MenuScreen::hide()
{

}

void MenuScreen::render()
{
    ALLEGRO_TRANSFORM tr;
    al_identity_transform(&tr);
    al_build_transform(&tr, 0, 0, 2, 2, 0);
    al_use_transform(&tr);

    aether::graphics::clear(1, 0, 0);
    m_game->assets.bigFont.print("The Iglesier", 68, 30, aether::graphics::Color(255, 0, 0));
    m_game->assets.font.print("Press ENTER to start", 68, 70, aether::graphics::Color(255, 255, 255));

    m_game->assets.alltiles_sheet.getFrame(9).draw(40, 124);
    m_game->assets.font.print("You must save as many parishioners as you can. Click them to ascend.", 50+18, 120, 180, 14, aether::graphics::Color(255, 255, 255), true);

    m_game->assets.alltiles_sheet.getFrame(1).draw(40, 170);
    m_game->assets.font.print("Build banks so parishioners can sit and pray", 50+18, 170, 180, 14, aether::graphics::Color(255, 255, 255), true);

    m_game->assets.alltiles_sheet.getFrame(7).draw(40, 210);
    m_game->assets.alltiles_sheet.getFrame(10).draw(40, 236);
    m_game->assets.alltiles_sheet.getFrame(8).draw(40, 262);

    m_game->assets.font.print("Build prayers so that parishers ascend to heaven automatically. The more sit parishers, the less their power.", 50+18, 210, 180, 14, aether::graphics::Color(255, 255, 255), true);



}


void MenuScreen::update(uint64_t delta)
{
    if( aether::core::is_key_just_pressed(aether::core::KeyCode::Enter) )
    {
        m_game->setScreen(&m_game->gameplayScreen);
    }
}
