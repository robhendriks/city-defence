//
// Created by Leendert on 14-11-2016.
//

#ifndef CITY_DEFENCE_JSON_MAP_LOADER_H
#define CITY_DEFENCE_JSON_MAP_LOADER_H


#include <fstream>
#include <json.hpp>
#include "json_level_loader.h"
#include "base_level_loader.h"
#include "../../config/json_config.h"
#include "../../domain/map/map.h"
#include "../../engine/math/vec2.hpp"

using json = nlohmann::json;
using namespace engine::math;

namespace services {
    namespace level_loader {
        class json_level_loader : public base_level_loader {
        public:
            json_level_loader(json root);

            std::unique_ptr<domain::game_level::game_level> load();
        private:
            void load_fields(json &root, domain::map::map &map1);

            void load_objects(json &root, domain::map::map &map1);

            json m_root;
        };
    };
};


#endif //CITY_DEFENCE_JSON_MAP_LOADER_H