//
// Created by te on 14-Nov-16.
//

#ifndef CITY_DEFENCE_ABSTRACT_DRAWABLE_GAME_OBJECT_H
#define CITY_DEFENCE_ABSTRACT_DRAWABLE_GAME_OBJECT_H

#include "../../engine/graphics/texture_manager.h"
#include "../../engine/math/box2.hpp"

namespace domain {
    namespace drawable{
        class abstract_drawable_game_object{
        public:
            virtual void draw(engine::graphics::texture_manager &texture_manager, engine::math::box2_t &dest) = 0;
            virtual void unload(engine::graphics::texture_manager &texture_manager)  = 0;
        };
    }
}
#endif //CITY_DEFENCE_ABSTRACT_DRAWABLE_GAME_OBJECT_H