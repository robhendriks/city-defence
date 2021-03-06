//
// Created by Mark on 5-12-2016.
//

#ifndef CITY_DEFENCE_DEFENSIVE_BUILDING_H
#define CITY_DEFENCE_DEFENSIVE_BUILDING_H

#include "building.h"

namespace domain {
    namespace map {
        namespace objects {

            class defensive_building : public building, public combat::attacker {
            public:
                defensive_building(const engine::math::box2_t &box,
                                   const std::string &id,
                                   int hitpoints,
                                   double health_ragen,
                                   const std::string &name,
                                   const std::vector<resources::resource*> &required_resources,
                                   int range, int min_damage, int max_damage);

                dragable_field_object *clone() const;

                void draw(drawable::draw_managers_wrapper &draw_managers, unsigned int time_elapsed);

                void update(domain::game_level::game_level &game_level, unsigned int elapsed_time);

                virtual void update_game_stats(domain::game_level::game_stats &game_stats1, std::string action);

            };

        }
    }
}

#endif //CITY_DEFENCE_DEFENSIVE_BUILDING_H
