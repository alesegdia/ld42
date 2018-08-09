#pragma once

#include <aether/aether.h>

class Assets
{
public:
    void load()
    {
        sample_texture.load("assets/sampleimg.png");
        sample_sheet.load(2, 2, sample_texture);
        sample_anim.addFrame(sample_sheet.getFrame(0), 1e6);
        sample_anim.addFrame(sample_sheet.getFrame(1), 2e6);
        sample_anim.addFrame(sample_sheet.getFrame(2), 3e6);
        sample_anim.addFrame(sample_sheet.getFrame(3), 1e6);
    }

    void cleanup()
    {
        sample_texture.destroy();
    }

    aether::graphics::Texture sample_texture;
    aether::graphics::Spritesheet sample_sheet;
    aether::graphics::Animation sample_anim;

};
