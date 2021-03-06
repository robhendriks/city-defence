//
// Created by robbie on 9-12-2016.
//

#ifndef CITY_DEFENCE_HELP_H
#define CITY_DEFENCE_HELP_H

#include "../../domain/drawable/draw_managers_wrapper.h"
#include "base_view.h"
#include "../../engine/input/mouse_buttons.h"
#include "../../engine/events/mouse_button_down.h"
#include "top_bar.h"

namespace gui {
    namespace views {
        class top_bar;
    }
}

namespace gui {
    namespace views {
        class help : public base_view,
                     engine::eventbus::subscriber<engine::events::mouse_button_down<engine::input::mouse_buttons::LEFT>>,
                     engine::eventbus::subscriber<engine::events::key_down> {
            friend class in_game_menu;
            friend class level;
            friend class win_game_over;
            friend class main_menu;
            friend class reclame;
        public:
            help(top_bar &top_bar1);

            void set_image(std::string image);

            void toggle_show();

            void before();

            void on_display_change(engine::math::box2_t display_box);

            void draw(unsigned int time_elapsed, engine::math::box2_t display_box);

            void after();

            void on_event(engine::events::mouse_button_down<engine::input::mouse_buttons::LEFT> &event);

            void on_event(engine::events::key_down &event);

            void call_on_show_change(std::function<void(bool)> callback);

        private:
            top_bar &m_top_bar;
            std::string m_image;
            bool m_show;
            bool m_help_loaded;
            std::unique_ptr<engine::math::box2_t> m_overlay_box;
            std::unique_ptr<engine::math::box2_t> m_help_box;
            std::unique_ptr<engine::math::box2_t> m_image_box;
            std::unique_ptr<engine::math::box2_t> m_cross_circle_box;
            std::unique_ptr<engine::math::box2_t> m_cross_box;
            std::vector<std::function<void(bool)>> m_callbacks;
        };
    }
}

#endif //CITY_DEFENCE_HELP_H
