#include "endscreen.h"

#include "../ld42.h"


EndScreen::EndScreen(LD42 *game)
{
    m_game = game;
}

void EndScreen::show()
{

}

void EndScreen::hide()
{

}

void EndScreen::render()
{
    aether::graphics::clear(1, 0, 0);

    static char saved_text_buffer[32];
    sprintf(saved_text_buffer, "x %d", m_game->gameplayScreen.scene()->parishersSaved());
    m_game->assets.bigFont.print("Game Over", 68, 60, aether::graphics::Color(255, 255, 255));
    m_game->assets.font.print("Parishioners saved", 72, 180, aether::graphics::Color(255, 255, 255));
    m_game->assets.font.print(saved_text_buffer, 120, 200, aether::graphics::Color(255, 255, 255));

    if(m_game->gameplayScreen.status() == Scene::Status::LostApocalypsisFullBar)
    {
        m_game->assets.font.print("The Apocalypsis reached its culmination", 8, 100, aether::graphics::Color(255, 255, 255));
    }
    else if( m_game->gameplayScreen.status() == Scene::Status::LostTooMuchParishers)
    {
        m_game->assets.font.print("You left too many parishioners outside", 12, 100, aether::graphics::Color(255, 255, 255));
    }

    m_game->assets.font.print("Press ENTER to restart", 62, 250, aether::graphics::Color(255, 255, 255));
}


void EndScreen::update(uint64_t delta)
{
    if( aether::core::is_key_just_pressed(aether::core::KeyCode::Enter) )
    {
        m_game->setScreen(&m_game->menuScreen);
    }
}
