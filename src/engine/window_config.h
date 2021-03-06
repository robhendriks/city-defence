//
// Created by Rob Hendriks on 19/10/2016.
//

#ifndef CITY_DEFENCE_WINDOW_CONFIG_H
#define CITY_DEFENCE_WINDOW_CONFIG_H

#include <SDL.h>
#include "window.h"
#include "graphics/color4.hpp"

namespace engine {
    struct window_config {
        window_config(std::string title, graphics::color4_t bg_color) : title(title), background_color(bg_color) {}
        std::string title;
        graphics::color4_t background_color;
        int x = SDL_WINDOWPOS_CENTERED;
        int y = SDL_WINDOWPOS_CENTERED;

        // Use -1 for full width/height
        int w = -1;
        int h = -1;

        Uint32 flags = SDL_WINDOW_SHOWN;

        bool debug = false;
    };
}

#endif //CITY_DEFENCE_WINDOW_CONFIG_H
