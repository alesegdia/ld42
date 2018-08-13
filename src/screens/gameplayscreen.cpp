#include "gameplayscreen.h"

#include "../ld42.h"


GameplayScreen::GameplayScreen(LD42 *game)
    : m_game(game)
{

}

void GameplayScreen::show()
{
    m_scene.reset(new Scene(&m_game->assets.alltiles_sheet));
}

void GameplayScreen::hide()
{

}

void GameplayScreen::render()
{
    aether::graphics::clear(1, 0, 0);

    ALLEGRO_TRANSFORM tr;
    al_identity_transform(&tr);
    al_build_transform(&tr, 0, 0, 2, 2, 0);
    al_use_transform(&tr);

    m_scene->render();

    static char favor_text_buffer[32];
    static char saved_text_buffer[32];
    static char wcost_text_buffer[32];
    sprintf(favor_text_buffer, "God's favor x %d", m_scene->godFavor());
    sprintf(saved_text_buffer, "Parishioners saved x %d", m_scene->parishersSaved());
    sprintf(wcost_text_buffer, "Cost: %d", m_scene->selectedWidget().price());
    m_game->assets.font.print("Iglesier: The Apocalyptic Saver Cure", 4, 0, aether::graphics::Color(255, 0, 0));
    m_game->assets.font.print(favor_text_buffer, 4, 16, aether::graphics::Color(255, 255, 255));
    m_game->assets.font.print(saved_text_buffer, 4, 32, aether::graphics::Color(255, 255, 255));
    m_game->assets.font.print(wcost_text_buffer, 64, 48, aether::graphics::Color(255, 255, 255));
    m_game->assets.font.print("Apocalypsis Level: ", 2, 304, aether::graphics::Color(255, 255, 255));
}


void GameplayScreen::update(uint64_t delta)
{
    if( aether::core::is_mouse_button_just_pressed(1) )
    {
        m_scene->clicked();
    }
    else if( aether::core::is_mouse_button_just_pressed(2) )
    {
        m_scene->clicked(false);
    }

    m_scene->tryAccomodateQueuedParishers();
    m_scene->update(delta);

    if( m_scene->status() != Scene::Status::Running )
    {
        m_game->setScreen(&m_game->endScreen);
    }

}
