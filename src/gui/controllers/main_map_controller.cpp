//
// Created by te on 01-Nov-16.
//

#include "main_map_controller.h"

namespace gui {
    namespace controllers {
        main_map_controller::main_map_controller(views::level &view, engine::engine &engine,
                                                 views::win_game_over &transition_view, models::main_map_model &model,
                                                 models::transition_level_model &transition_model,
                                                 models::level_goals_model &level_goals_model, game &game1,
                                                 services::wave::wave_management &wave_management,
                                                 services::level_loader::base_level_loader &level_loader) :
                base_controller(game1), m_view(view), m_trans_view(transition_view), m_engine(engine), m_model(model),
                m_trans_model(transition_model), m_level_goals_model(level_goals_model),
                m_wave_management_service(wave_management), m_level_loader(level_loader) {
            m_view.set_controller(*this);
            m_trans_view.set_controller(*this);

            m_previous_time = 0;
        }

        void main_map_controller::show() {
            auto &lvl = m_model.world->get_current_level();
            if (lvl.get_start_time() == 0) {
                lvl.set_start_time(m_engine.get_time_elapsed());
            }

            m_level_goals_model.game_goals = &lvl.get_goal();
            m_level_goals_model.game_stats = &lvl.get_stats();

            if (is_lvl_done()) {
                m_engine.reset_speed();
                m_model.paused = true;

                m_trans_model.duration = m_engine.get_time_elapsed() - lvl.get_start_time();
                m_trans_model.result = !lvl.is_game_over(m_engine.get_time_elapsed());
                m_trans_model.next_lvl_exists =
                        m_level_loader.get_level_count() > m_model.world->get_current_level().get_id() + 1;
                view(m_trans_view);
            } else {
                m_model.paused = false;
                view(m_view);
            }
        }

        bool main_map_controller::is_lvl_done() {
            auto &lvl = m_model.world->get_current_level();

            bool game_over = lvl.is_game_over(m_engine.get_time_elapsed());
            bool goal_reached = lvl.is_goal_reached();

            return game_over || goal_reached;
        }

        // this needs to be handled by eventbus
        void main_map_controller::update() {

            auto current_enemies = m_model.world->get_current_level().get_enemies_in_lvl();
            for (auto &enemy : m_wave_management_service.get_enemies(m_engine.get_time_elapsed())) {
                current_enemies.push_back(enemy);
            }

            std::vector<domain::nations::enemy*> disposed;

            for (auto &enemy : current_enemies) {
                enemy->update(m_engine.get_time_elapsed());
                if (enemy->is_disposed()) {
                    disposed.push_back(enemy);
                }
            }

            for (auto &dispose : disposed) {
                m_model.world->get_current_level().remove_enemy_in_lvl(*dispose);
            }

            auto fields = m_model.world->get_current_level().get_map().get_fields();

            for (auto &field : fields) {
                if (!field) {
                    continue;
                }

                auto placeable_object = field->get_object();
                if (placeable_object) {
                    auto defensive_building = dynamic_cast<domain::map::objects::defensive_building*>(placeable_object);
                    if (defensive_building) {
                        defensive_building->update(m_model.world->get_current_level(), m_engine.get_time_elapsed());
                    }
                }
            }

//            m_model.world->get_current_level().remove_placeable_object()

            m_model.world->get_current_level().set_enemies_in_lvl(current_enemies);
            //Updates building, calles method within the level
            if (m_engine.get_time_elapsed() > m_previous_time + 500) {
                //Updating
                m_model.world->get_current_level().update();
                m_previous_time = m_engine.get_time_elapsed();
            }
        }

        void main_map_controller::set_game_world(domain::gameworld::game_world &game_world) {
            m_model.world = &game_world;

            // set wave service values to first lvl
            set_settings_wave_management_service(m_model.world->get_current_level());
        }

        void main_map_controller::next_lvl() {
            m_wave_management_service.reset();

            auto current_level_id = m_model.world->get_current_level().get_id();
            // count = from 1 and id = from 0 so + 1
            if (m_level_loader.get_level_count() > current_level_id + 1) {
                m_model.world->set_current_level(*m_level_loader.load(current_level_id + 1));

                // set wave service values to next lvl
                set_settings_wave_management_service(m_model.world->get_current_level());
                show();
            } else {
                // this saves the stats of the last lvl and sets current to nullptr
                //m_model.world->set_current_level(nullptr);

                m_menu_controller->show();
            }
        }

        void main_map_controller::pause() {
            // Let the level know
            m_model.world->get_current_level().pause();

            // Pause or resume the engine
            if (m_engine.get_state() == engine::PAUSED) {
                m_engine.resume();
                m_model.paused = false;
            } else {
                m_engine.pause();
                m_model.paused = true;
            }
        };

        /**
         * Called after a level is done or stopped
         */
        void main_map_controller::resume_engine_if() {
            if (m_engine.get_state() == engine::PAUSED) {
                m_engine.resume();
            }
        }

        void main_map_controller::set_menu_controller(gui::controllers::menu_controller &menu_controller) {
            m_menu_controller = &menu_controller;
        }

        void main_map_controller::set_settings_wave_management_service(domain::game_level::game_level &lvl) {
            m_wave_management_service.set_peace_period(lvl.get_peace_period());
            m_wave_management_service.set_base_wave_opportunity(lvl.get_base_wave_base_opportunity());
            m_wave_management_service.set_wave_opportunity_increase(lvl.get_wave_opportunity_increase());
            m_wave_management_service.set_wave_spawn_time_range(lvl.get_wave_spawn_time_range());
            m_wave_management_service.set_waves_interval(lvl.get_waves_interval());
            m_wave_management_service.set_spawn_bosses(lvl.get_spawn_bosses());
            m_wave_management_service.set_spawnable_nation(lvl.get_enemy_nation());
            m_wave_management_service.get_wave_generator().get_ai().set_map(lvl.get_map());
        }
    }
}
