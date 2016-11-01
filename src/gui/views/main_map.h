//
// Created by te on 01-Nov-16.
//

#ifndef CITY_DEFENCE_MAIN_MAP_H
#define CITY_DEFENCE_MAIN_MAP_H

#include "../../engine/graphics/texture_manager.h"
#include "../../engine/events/mouse_button_down.h"
#include "../../engine/eventbus/subscriber.h"
#include "../../domain/gameworld/game_world.h"
#include "../../engine/window.h"
#include "../models/main_map_model.h"

namespace gui {
    namespace views {
        class main_map : public engine::eventbus::subscriber<engine::events::mouse_button_down<engine::input::mouse_buttons::LEFT>>{
        public:
            main_map(engine::graphics::texture_manager &texture_manager, engine::window &window);

            void before_first_draw();

            void draw(models::main_map_model &view_model);

            void after_last_draw();

            void on_event(engine::events::mouse_button_down<engine::input::mouse_buttons::LEFT> &event);
        private:
            engine::graphics::texture_manager& m_texture_manager;
            engine::window &m_window;
        };
    }
}
#endif //CITY_DEFENCE_MAIN_MAP_H