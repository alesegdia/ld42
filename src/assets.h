#pragma once

#include <aether/aether.h>

class Assets
{
public:
    void load()
    {
        alltiles_texture.load("assets/alltiles.png");
        alltiles_sheet.load(4, 4, alltiles_texture);

        font.load("assets/Alkhemikal.ttf", 16);
        bigFont.load("assets/Alkhemikal.ttf", 32);

        theme.load("assets/iglesiertheme.ogg");
    }

    void cleanup()
    {
        alltiles_texture.destroy();
        font.destroy();
        bigFont.destroy();
        theme.destroy();
    }

    aether::graphics::Texture alltiles_texture;
    aether::graphics::Spritesheet alltiles_sheet;

    aether::graphics::Font font;
    aether::graphics::Font bigFont;

    aether::audio::Stream theme;

};
