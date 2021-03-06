#include "engine/engine.h"
#include "gui/controllers/menu_controller.h"
#include "boost/di.hpp"
#include "config/json_config.h"
#include "utils/string_utils.h"
#include "domain/map/ai/states/move_state.h"
#include "domain/map/ai/states/search_and_destroy_state.h"
#include "services/world_loader/json_world_loader.h"
#include "services/world_saver/json_world_saver.h"

int main(int argc, char *argv[]) {
    /**
     * CONFIG
     */
    auto *json_config = new config::json_config("config.json");
    json_config->load();

    engine::window_config w_config = {
        json_config->get_string("engine.title"),
        engine::graphics::color4_t(utils::string_utils::hex_to_long(json_config->get_string("window.background_color")))
    };

    w_config.w = json_config->get_int("window.width", w_config.w);
    w_config.h = json_config->get_int("window.height", w_config.h);
    w_config.debug = json_config->get_bool("window.debug", w_config.debug);

    engine::engine_config e_config = { w_config };

    // Create a font_manager with fonts
    auto *font_manager = new engine::graphics::font_manager();
    font_manager->add("fonts/Roboto-Regular.ttf", "roboto");

    /**
     * END OF CONFIG
     */

    // Get the engine
    engine::engine *engine1 = new engine::engine(e_config);
    engine1->warmup();

    // Create the ioc container
    auto *game1 = new game(*engine1->get_window());

    data::json::save_games_json_repository *save_games_json_repository = new data::json::save_games_json_repository();
    services::world_loader::json_world_loader *world_loader = new services::world_loader::json_world_loader();
    services::world_saver::json_world_saver *world_saver = new services::world_saver::json_world_saver(*save_games_json_repository);

    auto di_config = [&]() {
        return boost::di::make_injector(
                boost::di::bind<>.to(*game1),
                boost::di::bind<>.to(*engine1),
                boost::di::bind<>.to(*engine1->get_texture_manager()),
                boost::di::bind<>.to(*engine1->get_color_manager()),
                boost::di::bind<>.to(*engine1->get_sound_manager()),
                boost::di::bind<>.to(*engine1->get_music_manager()),
                boost::di::bind<>.to(*engine1->get_window()),
                boost::di::bind<>.to(*font_manager),
                boost::di::bind<services::world_loader::base_world_loader>.to(*world_loader),
                boost::di::bind<services::world_saver::base_world_saver>.to(*world_saver),
                boost::di::bind<data::json::save_games_json_repository>.to(*save_games_json_repository)
        );
    };

    /**
     *  ADD SUBSCRIBERS
     */
    auto &eventbus = engine::eventbus::eventbus::get_instance();

    // Subscribe the game to the window cleared event and display changed events
    eventbus.subscribe(dynamic_cast<engine::eventbus::subscriber<engine::events::window_cleared>*>(game1));
    eventbus.subscribe(dynamic_cast<engine::eventbus::subscriber<engine::events::display_changed>*>(game1));

    /**
     * END OF SUBSCRIBERS REGISTRY
     */

    // Start with show
    gui::controllers::menu_controller* menu_controller = boost::di::make_injector(di_config()).create<gui::controllers::menu_controller*>();
    menu_controller->show();

    // Run the game
    engine1->run();

    // Stop/cooldown the engine
    delete font_manager;
    engine1->cooldown();

    // More cleaning
    delete game1;
    delete menu_controller;
    delete json_config;
    delete engine1;
    delete save_games_json_repository;
    delete world_loader;
    delete world_saver;

    return 0;
}
