//
// Created by robbie on 29-10-2016.
//

#ifndef CITY_DEFENCE_MENU_CONTROLLER_H
#define CITY_DEFENCE_MENU_CONTROLLER_H

#include "../../engine/engine.h"
#include "../views/main_menu.h"
#include "base_controller.h"
#include "main_map_controller.h"
#include "credits_controller.h"
#include "../../services/level_loader/base_level_loader.h"
#include <memory>

namespace gui {
    namespace views {
        class main_menu;
    }

    namespace controllers {
        class main_map_controller;
    }
}

namespace gui {
    namespace controllers {
        class menu_controller : public base_controller {
        public:
            menu_controller(views::main_menu &main_menu, engine::engine &engine,
                            controllers::main_map_controller &main_map_controller,
                            controllers::credits_controller &credits_controller, game &game1,
                            services::level_loader::base_level_loader &map_loader);

            void show();

            void play();

            void load();

            void quit();

            void credits();

        private:
            engine::engine &m_engine;
            views::main_menu &m_main_menu;
            controllers::main_map_controller &m_main_map_controller;
            controllers::credits_controller &m_credits_controller;
            services::level_loader::base_level_loader &m_level_loader;
        };
    }
}

#endif //CITY_DEFENCE_MENU_CONTROLLER_H
