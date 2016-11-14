//
// Created by te on 25-Oct-16.
//

#ifndef CITY_DEFENCE_BASE_PLACEABLE_OBJECT_H
#define CITY_DEFENCE_BASE_PLACEABLE_OBJECT_H

#include "../drawable/drawable_game_object.h"

namespace domain {
    namespace buildings {
        class base_placeable_object : public domain::drawable::drawable_game_object {
        public:
            base_placeable_object(const std::string &id, const std::string &file_loc,
                                  engine::math::vec2_t *image_start_position);
            virtual ~base_placeable_object() = default;
        };
    }
}

#endif //CITY_DEFENCE_BASE_PLACEABLE_OBJECT_H
