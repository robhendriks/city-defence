//
// Created by robbie on 29-10-2016.
//

#include "menu_controller.h"

namespace gui {
    namespace controllers {

        menu_controller::menu_controller(views::main_menu &main_menu, engine::engine &engine,
                                         controllers::main_map_controller &main_map_controller, game &game1
                , services::level_loader::base_map_loader &map_loader)
            : base_controller(game1), m_engine(engine), m_main_menu(main_menu),
              m_main_map_controller(main_map_controller), _map_loader(map_loader){

            m_main_menu.set_controller(*this);
        }

        void menu_controller::show() {
            view(m_main_menu);
            m_main_menu.set_controller(*this);
        }

        void menu_controller::play() {
            //            TODO: load map (level1)
            _game_world = std::unique_ptr<domain::gameworld::game_world>(new domain::gameworld::game_world(this->_map_loader.load("level1.json")));
            domain::map::map* test = (domain::map::map*) _game_world->get_current_map();
            test->get_tile_height();
            m_main_map_controller.set_game_world(std::move(_game_world));
            m_main_map_controller.show();
        }

        void menu_controller::load() {
            play();
        }

        void menu_controller::quit() {
            m_engine.stop();
        }
    }
}
