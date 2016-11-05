#include "engine/engine.h"
#include "gui/controllers/menu_controller.h"
#include "boost/di.hpp"
#include "config/json_config.h"
#include "utils/string_utils.h"
#include "gui/views/main_map.h"
#include "gui/controllers/main_map_controller.h"

int main(int argc, char *argv[]) {

    // Config
    auto *json_config = new config::json_config("config.json");
    json_config->load();

    engine::window_config w_config = {
        json_config->get_string("engine.title"),
        engine::graphics::color4_t(utils::string_utils::hex_to_long(json_config->get_string("window.background_color")))
    };

    w_config.w = json_config->get_int("window.width", w_config.w);
    w_config.h = json_config->get_int("window.height", w_config.h);

    engine::engine_config e_config = { w_config };

    // Get the engine
    engine::engine *engine1 = new engine::engine(e_config);
    engine1->warmup();

    // Create the ioc container
    auto *texture_manager = engine1->get_texture_manager();
    auto *color_manager = engine1->get_color_manager();
    auto *window = engine1->get_window();
    auto *game1 = new game();
    auto injector = boost::di::make_injector(
        boost::di::bind<>.to(*game1),
        boost::di::bind<>.to(*engine1),
        boost::di::bind<>.to(*texture_manager),
        boost::di::bind<>.to(*window)
    );

    // Subscribe the game to the window cleared event
    engine::eventbus::eventbus<engine::events::window_cleared>::get_instance().subscribe(game1);

    // Start with show
    auto *menu_controller = injector.create<gui::controllers::menu_controller*>();
    menu_controller->show();

    // Run the game
    engine1->run();

    // Clean
    delete game1;
    delete menu_controller;
    delete json_config;
    delete texture_manager;
    delete color_manager;
    delete window;
    delete engine1;

    return 0;
}
