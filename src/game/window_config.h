//
// Created by Rob Hendriks on 19/10/2016.
//

#ifndef CITY_DEFENCE_WINDOW_CONFIG_H
#define CITY_DEFENCE_WINDOW_CONFIG_H

#include <SDL.h>

namespace game {
    struct window_config {
        const char *title;
        int x = SDL_WINDOWPOS_CENTERED;
        int y = SDL_WINDOWPOS_CENTERED;
        int w = 1024;
        int h = 768;
        Uint32 flags = SDL_WINDOW_SHOWN;
    };
}

#endif //CITY_DEFENCE_WINDOW_CONFIG_H
