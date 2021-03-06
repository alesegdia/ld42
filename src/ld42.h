#include <aether/aether.h>

#include "screens/menu.h"
#include "screens/gameplayscreen.h"
#include "screens/endscreen.h"
#include "assets.h"

class LD42 : public aether::core::Application
{
public:
    LD42(int w, int h);

    int ready(int argc, char **argv) override;

    void update(uint64_t delta) override;

    void dispose() override;

    MenuScreen menuScreen;
    GameplayScreen gameplayScreen;
    EndScreen endScreen;

    Assets assets;

    int level = 1;
};

