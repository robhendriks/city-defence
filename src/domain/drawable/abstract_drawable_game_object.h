//
// Created by te on 14-Nov-16.
//

#ifndef CITY_DEFENCE_ABSTRACT_DRAWABLE_GAME_OBJECT_H
#define CITY_DEFENCE_ABSTRACT_DRAWABLE_GAME_OBJECT_H

#include "../../engine/math/box2.hpp"
#include "draw_managers_wrapper.h"

namespace domain {
    namespace drawable {
        class abstract_drawable_game_object {
        public:
            abstract_drawable_game_object() = default;

            virtual ~abstract_drawable_game_object() {}

            virtual void draw(draw_managers_wrapper &draw_managers, unsigned int time_elapsed) = 0;

            virtual void unload(draw_managers_wrapper &draw_managers)  = 0;
        };
    }
}
#endif //CITY_DEFENCE_ABSTRACT_DRAWABLE_GAME_OBJECT_H
