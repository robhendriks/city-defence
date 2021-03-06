//
// Created by robbie on 1-12-2016.
//

#ifndef CITY_DEFENCE_TOP_BAR_H
#define CITY_DEFENCE_TOP_BAR_H

#include "base_view.h"
#include "../../engine/input/mouse_buttons.h"
#include "../../engine/events/mouse_button_down.h"
#include "../controllers/menu_controller.h"
#include "../../domain/drawable/draw_managers_wrapper.h"

namespace gui {
    namespace controllers {
        class menu_controller;
    }
}

namespace gui {
    namespace views {
        class top_bar : public base_view,
                        engine::eventbus::subscriber<engine::events::mouse_button_down<engine::input::mouse_buttons::LEFT>>,
                        engine::eventbus::subscriber<engine::events::key_down> {
            friend class main_menu;
            friend class in_game_menu;
            friend class level;
            friend class win_game_over;
            friend class credits;
            friend class help;
            friend class reclame;
            friend class load;
            friend class cli;
        public:
            top_bar(domain::drawable::draw_managers_wrapper &draw_managers, engine::engine &engine);

            void set_controller(controllers::menu_controller &menu_controller);

            void before();

            void on_display_change(engine::math::box2_t display_box);

            void draw(unsigned int time_elapsed, engine::math::box2_t display_box);

            void after();

            void on_event(engine::events::mouse_button_down<engine::input::mouse_buttons::LEFT> &event);

            void on_event(engine::events::key_down &event);

        private:
            domain::drawable::draw_managers_wrapper &m_draw_managers;
            engine::engine &m_engine;
            engine::graphics::texture_manager &m_texture_manager;
            engine::graphics::color_manager &m_color_manager;
            engine::graphics::font_manager &m_font_manager;
            std::unique_ptr<engine::math::box2_t> m_bar_box;
            std::unique_ptr<engine::math::box2_t> m_close_box;
            controllers::menu_controller *m_menu_controller;

            unsigned int m_previous_draw_time;
            unsigned int m_fps;
            bool m_fps_show;
        };
    }
}


#endif //CITY_DEFENCE_TOP_BAR_H
