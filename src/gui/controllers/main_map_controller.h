//
// Created by te on 01-Nov-16.
//

#ifndef CITY_DEFENCE_MAIN_MAP_CONTROLLER_H
#define CITY_DEFENCE_MAIN_MAP_CONTROLLER_H


#include "base_controller.h"
#include "../../engine/engine.h"
#include "../models/main_map_model.h"
#include "../models/transition_level_model.h"
#include "../models/level_goals_model.h"
#include "../../domain/gameworld/game_world.h"
#include "../views/level.h"
#include "../views/win_game_over.h"
#include "menu_controller.h"
#include "../../services/wave/wave_management.h"
#include "../../domain/map/objects/defensive_building.h"
#include "../../data/json/highscore_json_repository.h"
#include "../../services/world_saver/base_world_saver.h"

namespace gui {
    namespace views {
        class level;

        class win_game_over;
    }

    namespace controllers {
        class menu_controller;
    }
}

namespace gui {
    namespace controllers {
        class main_map_controller : public base_controller {
        public:
            main_map_controller(views::level &view, engine::engine &engine, views::win_game_over &transition_view,
                                models::main_map_model &model, models::transition_level_model &transition_model,
                                models::level_goals_model &level_goals_model,
                                game &game1, services::wave::wave_management &wave_management,
                                data::json::highscore_json_repository &highscore_repository,
                                services::world_saver::base_world_saver &world_saver);

            void show();

            domain::gameworld::game_world *get_game_world();

            void set_game_world(domain::gameworld::game_world &game_world);

            void set_menu_controller(gui::controllers::menu_controller &menu_controller);

            void next_lvl();

            bool is_lvl_done();

            void update();

            void pause();

            void resume_engine_if();

            void save();

        private:
            gui::controllers::menu_controller *m_menu_controller;
            views::level &m_view;
            views::win_game_over &m_trans_view;
            engine::engine &m_engine;
            models::main_map_model &m_model;
            models::transition_level_model &m_trans_model;
            models::level_goals_model &m_level_goals_model;
            services::wave::wave_management &m_wave_management_service;
            unsigned int m_previous_time;
            data::json::highscore_json_repository &m_highscore_repository;
            services::world_saver::base_world_saver &m_world_saver;

            // set values of the wave_management_service
            void set_settings_wave_management_service(domain::game_level::game_level &lvl);
        };
    }
}

#endif //CITY_DEFENCE_MAIN_MAP_CONTROLLER_H
