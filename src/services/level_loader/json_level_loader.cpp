//
// Created by robbie on 20-11-2016.
//

#include <fstream>
#include "json_level_loader.h"
#include "../../domain/map/objects/building.h"
#include "../../domain/map/objects/road.h"

namespace services {
    namespace level_loader {

        json_level_loader::json_level_loader(json root) : m_root(root) {

        }

        std::unique_ptr<domain::game_level::game_level> json_level_loader::load() {
            // Create the map
            float width = m_root["width"];
            float height = m_root["height"];
            auto map1 = std::shared_ptr<domain::map::map>(new domain::map::map({width - 1, height - 1}, {32, 32}));

            // Load the fields
            load_fields(m_root, *map1);
            // Load the objects
            load_objects(m_root, *map1);

            // Create the level
            auto *d_a_d = new engine::draganddrop::drag_and_drop();
            auto goal = std::shared_ptr<domain::game_level::game_stats>(new domain::game_level::game_stats(3, 0, 10000));
            auto game_level = std::unique_ptr<domain::game_level::game_level>(
                new domain::game_level::game_level("level", map1, goal, std::vector<std::pair<int, std::shared_ptr<domain::nations::enemy>>>(), *d_a_d));

            // TODO: HARDCODED ATM
            // Add placeable objects
            engine::math::box2_t building_box {{10, 10}, {42, 42}};
            auto *building = new domain::map::objects::building(building_box);
            building->set_draw_settings("images/building-a.png");
            game_level->add_placeable_object(*building);

            // Done with building the level
            return game_level;
        }

        void json_level_loader::load_fields(json &root, domain::map::map &map1) {
            if (root.find("tiles") == root.end()) {
                return;
            }

            json tiles = root["tiles"];
            if (!tiles.is_object()) {
                return;
            }

            size_t tileCount = tiles["count"];
            SDL_Log("Loading %d tile(s)...\n", tileCount);

            json data = tiles["data"];
            if (!data.is_array()) {
                return;
            }

            for (json &elem : data) {
                int x = elem["x"];
                int y = elem["y"];
                int type = elem["type"];

                SDL_Log("%d %d %d", x, y, type);

                // Create the field, note that the field will automatically be added to the map
                auto *field = new domain::map::field(map1, {(float)x, (float)y});
                field->set_draw_settings("images/grass.png");
            }
        }

        void json_level_loader::load_objects(json &root, domain::map::map &map1) {
            if (root.find("objects") == root.end()) {
                return;
            }

            json level_objects = root["objects"];
            if (!level_objects.is_object()) {
                return;
            }

            size_t level_objects_count = level_objects["count"];
            SDL_Log("Loading %d level objects(s)...\n", level_objects_count);

            json data = level_objects["data"];
            if (!data.is_array()) {
                return ;
            }

            // Loop through all objects
            for (json &elem : data) {
                std::string id = elem["id"];
                int x = elem["x"];
                int y = elem["y"];
                int rotation = elem["rotation"];

                auto field = map1.get_field({(float)x, (float)y});
                if (field) {
                    domain::map::objects::object *object = nullptr;

                    std::string image_location;

                    std::string building_str = "building";
                    if (std::mismatch(building_str.begin(), building_str.end(), id.begin()).first == building_str.end()) {
                        object = new domain::map::objects::building(field.get());

                        image_location = "images/";
                        image_location += id;
                        image_location += ".png";
                    } else {
                        domain::map::objects::type type1;
                        if (id == "road-straight") {
                            type1 = domain::map::objects::type::STRAIGHT;
                            image_location = "images/road-straight.png";
                        } else if (id == "road-junction") {
                            type1 = domain::map::objects::type::JUNCTION;
                            image_location = "images/road-junction.png";
                        } else if (id == "road-corner") {
                            type1 = domain::map::objects::type::CORNER;
                            image_location = "images/road-corner.png";
                        } else if (id == "road-t-junction") {
                            type1 = domain::map::objects::type::T_JUNCTION;
                            image_location = "images/road-t-junction.png";
                        } else {
                            // Road CAP
                            type1 = domain::map::objects::type::CAP;
                            image_location = "images/road-cap.png";
                        }

                        // Create the object
                        object = new domain::map::objects::road(field.get(), type1);
                    }

                    // Calculate the image start position
                    float image_start_y = map1.get_tile_size().y * rotation;

                    // Place the (created) object on the field
                    object->set_draw_settings(image_location, {0, image_start_y}, rotation);

                    field->place_object(*object);
                }

                SDL_Log("%d %d %d", x, y, rotation);
            }
        }
    }
}
