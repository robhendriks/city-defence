//
// Created by robbie on 20-11-2016.
//

#include <fstream>
#include "json_level_loader.h"
#include "../../domain/map/objects/road.h"

#include "../../domain/map/objects/defensive_building.h"
#include "../../domain/map/objects/economic_building.h"

namespace services {
    namespace level_loader {

        json_level_loader::json_level_loader(json root)
            : m_root(root) {}

        std::unique_ptr<domain::game_level::game_level> json_level_loader::load() {
            //load_level needs te change in the future
            int load_level_nr = 0;

            //load nations if not loaded yet.
            if (this->vec_nations.empty()) {
                vec_nations = load_nations(
                    m_root["nations-url"]);
            }

            //load buildings if not loaded yet
            if (this->vec_building.empty()) {
                std::string buildings = m_root["building-url"];
                load_buildings(buildings);
            }
            if (this->vec_building.empty()) {
                json buildings = m_root["buildings"];
                if (buildings.is_array()) {
                    for (json &building : buildings) {
                        vec_building.push_back(load_buildings(building));
                    }
                }
            }

            //load lvl src's if not loaded yet
            if (vec_levels.empty()) {
                json lvls = m_root["lvls"];
                if (lvls.is_array()) {
                    vec_levels.push_back(load_all_levels(lvls[load_level_nr]));
                }
            }

            // Create the level
            auto *d_a_d = new engine::draganddrop::drag_and_drop();
            auto goal = std::make_shared<domain::game_level::game_stats>();
            goal->set_counter("buildings", 5);
            auto game_level = std::unique_ptr<domain::game_level::game_level>(
                new domain::game_level::game_level("level", vec_levels[load_level_nr], goal, vec_nations.front(),
                                                   *d_a_d,
                                                   125000));
            for (auto building : vec_building) {
                game_level->add_placeable_object(*building);
            }

            return game_level;
        }

        std::vector<nation_ptr> json_level_loader::load_nations(std::string nation_url) {
            std::vector<nation_ptr> pre_nation_list;

            std::ifstream file(nation_url);
            if (!file.is_open()) {
                throw std::runtime_error(std::string("Unable to open file: ") + nation_url);
            }

            json nation_root;
            try {
                nation_root << file;

                for (json &elem : nation_root) {
                    auto current_nation = std::make_shared<domain::nations::nation>(
                        domain::nations::nation(elem["id"], "_"));

                    json data = elem["units"];
                    if (!data.is_array()) {
                        return pre_nation_list;
                    }
                    std::vector<std::shared_ptr<domain::nations::enemy>> pre_vec_enemies;
                    for (json &enemy : data) {
                        std::string enemy_name = enemy["name"];
                        double enemey_movement_speed = static_cast<double>(enemy["movement-speed"]);
                        int enemey_min_damage = static_cast<int>(enemy["min-damage"]);
                        int enemey_max_damage = static_cast<int>(enemy["max-damage"]);
                        int enemey_hitpoints = static_cast<int>(enemy["hitpoints"]);
                        std::string enemy_type = enemy["type"];
                        int enemey_oppertunity_cost = static_cast<int>(enemy["oppertunity-cost"]);

                        bool boss = false;
                        if (enemy_type == "boss") { boss = true; };
                        //TODO: not 100 dynamic
                        std::shared_ptr<domain::nations::enemy> curren_enemy =
                            std::make_shared<domain::nations::enemy>(
                                domain::nations::enemy(enemy_name, enemey_min_damage, enemey_max_damage, 2,
                                                       enemey_hitpoints, 100, 2, enemey_movement_speed, boss,
                                                       current_nation, enemey_oppertunity_cost));
                        curren_enemy->set_draw_settings("images/building-a.png");
                        pre_vec_enemies.push_back(curren_enemy);
                        current_nation->setavailableenemies(pre_vec_enemies);
                        pre_nation_list.push_back(current_nation);
                    }

                }
            } catch (std::exception &e) {
                // TODO: proper error handling
                auto d = e.what();
                std::cout << d;
                throw;
            }
            return pre_nation_list;
        }

        void json_level_loader::load_fields(json &root, domain::map::map &map1) {
            if (root.find("tiles") == root.end()) {
                return;
            }

            json elements = root["tiles"];
            if (!elements.is_array()) {
                return;
            }

            SDL_Log("Loading %zu tiles...\n", elements.size());

            int tile_width = 64;
            int tile_height = 64;
            int tileset_columns = 17; // 1088 / 64 = 17

            for (json &elem : elements) {
                int tile_x = elem.value("x", -1); // get x, default to -1
                int tile_y = elem.value("y", -1); // get y, default to -1
                int tile_id = elem.value("id", -1); // get id, default to -1

                // Report and continue if there's bogus data
                if (tile_x < 0 || tile_y < 0 || tile_id < 0) {
                    SDL_Log("Invalid data for tile at %i:%i.", tile_x, tile_y);
                    continue;
                }

                // Calculate the tile's sprite row and column
                int image_row = tile_id / tileset_columns;
                int image_column = tile_id % tileset_columns;

                // Create the field and add it to the map
                auto *field = new domain::map::field(map1, {
                    static_cast<float>(tile_x),
                    static_cast<float>(tile_y)});

                auto image_start_position = vec2_t{
                    static_cast<float>(image_column * tile_width),
                    static_cast<float>(image_row * tile_height)};

                field->set_draw_settings("images/tileset.png", image_start_position);
            }
        }

        void json_level_loader::load_objects(json &root, domain::map::map &map1) {
            if (root.find("objects") == root.end()) {
                return;
            }

            json elements = root["objects"];
            if (!elements.is_array()) {
                return;
            }

            SDL_Log("Loading %zu objects...\n", elements.size());
return;
            int column = 0;
            // Loop through all objects
            for (json &elem : elements) {
                std::string id = elem["id"];
                int x = elem["x"];
                int y = elem["y"];
                int rotation = elem["rotation"];

                auto field = map1.get_field({(float) x, (float) y});
                if (field) {
                    domain::map::objects::field_object *object = nullptr;

                    std::string image_location;

                    std::string building_str = "road";
                    if (std::mismatch(building_str.begin(), building_str.end(), id.begin()).first ==
                        building_str.end()) {
                        if (id == "road-straight") {
                            image_location = "images/road-straight.png";
                        } else if (id == "road-junction") {
                            image_location = "images/road-junction.png";
                        } else if (id == "road-corner") {
                            image_location = "images/road-corner.png";
                        } else if (id == "road-t-junction") {
                            image_location = "images/road-t-junction.png";
                        } else {
                            image_location = "images/road-cap.png";
                        }

                        // Create the object
                        object = new domain::map::objects::road(field);
                        object->set_max_column(1);
                    } else {
                        object = new domain::map::objects::building(field);

                        image_location = "images/";
                        image_location += "building-a";
                        image_location += ".png";
                        object->set_max_column(2);
                    }

                    // Calculate the image start position
                    //TODO: rotation with row
                    float image_start_y = map1.get_tile_size().y * rotation;

                    // Place the (created) object on the field
                    object->set_draw_settings(image_location, {0, image_start_y});
                    object->set_rotation(rotation);

                    object->set_current_column(column);
                    auto b = dynamic_cast<domain::map::objects::building *>(object);
                    if (b != nullptr) {
                        auto m = b->get_health_regen();
                        std::cout << m;
                    }
                    field->place_object(object);
                    column = column + 1 <= object->get_max_column() ? column + 1 : 1;
                }

                SDL_Log("%d %d %d", x, y, rotation);
            }
        }

        std::shared_ptr<domain::map::objects::building> json_level_loader::load_buildings(std::string url) {
            std::shared_ptr<domain::map::objects::building> building;
            std::ifstream file(url);
            if (!file.is_open()) {
                throw std::runtime_error(std::string("Unable to open file: ") + url);
            }

            json building_root;
            try {
                building_root << file;

                for (json &building_data : building_root) {

                    int min_dmg = 0;
                    int max_dmg = 0;
                    int range = 0;
                    int type = building_data["type"];
                    auto output_sources = std::shared_ptr<domain::resources::resource>();
                    auto costs = std::vector<std::shared_ptr<domain::resources::resource>>();
                    engine::math::box2_t building_box{{10, 10},
                                                      {42, 42}};

                    auto data_building_cost = building_data["cost"];
                    auto it = data_building_cost.begin();
                    for (json::iterator building_costs_item = data_building_cost.begin();
                         it != data_building_cost.end(); ++it) {

                        costs.push_back(std::make_shared<domain::resources::resource>(building_costs_item.key(),
                                                                                      building_costs_item.value()));
                    }


                    for (json &building_properties : building_data["properties"]) {

                        auto current_prop = building_properties.begin();
                        for (json::iterator building_property_item = building_properties.begin();
                             current_prop != building_properties.end(); ++current_prop) {
                            //type 2 = dmg building

                            if (type == 2) {
                                if (building_property_item.key() == "min-damage") {
                                    min_dmg = static_cast<int>(building_property_item.value());
                                } else if (building_property_item.key() == "max-damage") {
                                    max_dmg = static_cast<int>(building_property_item.value());
                                } else if (building_property_item.key() == "range") {
                                    range = static_cast<int>(building_property_item.value());
                                }


                            } else {

                                output_sources = std::make_shared<domain::resources::resource>(
                                    building_property_item.key(),
                                    building_property_item.value());

                            }

                        }
                    }
                    if (type == 1) {
                        std::shared_ptr<domain::map::objects::economic_building> economic_building = std::make_shared<domain::map::objects::economic_building>(
                            building_box,
                            building_data["id"],
                            static_cast<int>(building_data["hitpoints"]),
                            static_cast<double>(building_data["health-regen"]),
                            building_data["name"],
                            costs, output_sources);
                        economic_building->set_draw_settings("images/building-a.png");
                        vec_building.push_back(economic_building);
                    } else {
                        std::shared_ptr<domain::map::objects::defensive_building> defencive_building = std::make_shared<domain::map::objects::defensive_building>(
                            building_box,
                            building_data["id"],
                            static_cast<int>(building_data["hitpoints"]),
                            static_cast<double>(building_data["health-regen"]),
                            building_data["name"],
                            costs, min_dmg, max_dmg, range);
                        defencive_building->set_draw_settings("images/building-a.png");
                        vec_building.push_back(defencive_building);
                    }


                }
            } catch (std::exception &e) {
                // TODO: proper error handling
                auto d = e.what();
                std::cout << d;
                throw;
            }
            return building;
        }

        std::shared_ptr<domain::map::map> json_level_loader::load_all_levels(std::string url) {
            std::ifstream file(url);
            if (!file.is_open()) {
                throw std::runtime_error(std::string("Unable to open file: ") + url);
            }

            try {
                json map_root;
                map_root << file;

                int width = map_root.value("width", -1);
                int height = map_root.value("height", -1);

                if (width < 1 || height < 1) {
                    throw std::runtime_error(std::string("Invalid world dimensions in file: ") + url);
                }

                // Create the map (currently uses fixed tile dimensions)
                auto map = std::make_shared<domain::map::map>(
                    vec2_t{static_cast<float>(width - 1), static_cast<float>(height - 1)},
                    vec2_t{64, 64});

                load_fields(map_root, *map);
                load_objects(map_root, *map);

                return map;
            } catch (std::exception &e) {
                std::cout << e.what() << "\n";
                throw;
            }
        }

    }
}
