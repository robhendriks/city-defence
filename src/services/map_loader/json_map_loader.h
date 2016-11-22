//
// Created by Leendert on 14-11-2016.
//

#ifndef CITY_DEFENCE_JSON_MAP_LOADER_H
#define CITY_DEFENCE_JSON_MAP_LOADER_H


#include <fstream>
#include <json.hpp>
#include "json_map_loader.h"
#include "base_map_loader.h"
#include "../../domain/map/base_field.h"
#include "../../domain/map/passable_field.h"
#include "../../config/json_config.h"
#include "../../domain/map/map.h"
#include "../../engine/math/vec2.hpp"
#include "../../domain/buildings/building.h"

using json = nlohmann::json;
using namespace engine::math;

namespace services {
    namespace level_loader {
        class json_map_loader : public base_map_loader {

        public:
            json_map_loader();

            domain::gameworld::game_world load(std::string file_location);

            virtual ~json_map_loader();

        private:
            struct building_objects_with_pos {
                int X;
                int Y;
                domain::buildings::base_placeable_object* object;
            };
            struct road_objects_with_pos {
                int X;
                int Y;
                domain::buildings::base_placeable_object* object;
            };

            std::vector<domain::map::base_field *> load_tiles(json &root);

            vec2_t get_length_and_width(json &root);

            std::vector<building_objects_with_pos *> load_buildings(json &root);
            std::vector<road_objects_with_pos *> load_roads(json &root);
        };
    };
};


#endif //CITY_DEFENCE_JSON_MAP_LOADER_H
