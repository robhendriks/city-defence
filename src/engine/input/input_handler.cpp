//
// Created by robbie on 22-10-2016.
//

#include "input_handler.h"
#include "../eventbus/eventbus.h"
#include "../events/mouse_button_down.h"
#include "mouse_buttons.h"
#include "../events/mouse_button_up.h"
#include "../events/mouse_motion.h"
#include "../events/key_down.h"
#include "keycodes.h"

namespace engine {
    namespace input {

        input_handler* input_handler::m_instance = nullptr;

        input_handler::input_handler() {
            // Set the initial values
            reset();
        }

        /**
         * Get the instance of the input handler (Singleton)
         *
         * @return bool
         */
        input_handler *input_handler::get_instance() {
            if (m_instance == nullptr) {
                m_instance = new input_handler();
            }

            return m_instance;
        }

        void input_handler::reset() {
            // Reset the mouse states
            m_right_mouse_button_pressed = false;
            m_middle_mouse_button_pressed = false;
            m_right_mouse_button_pressed = false;

            m_key_states = nullptr;
            m_mouse_position = new math::vec2_t(0.0, 0.0);
        }

        /**
         * Whether the right mouse button is pressed at the moment
         *
         * @return bool
         */
        bool input_handler::is_right_mouse_button_pressed() const {
            return m_right_mouse_button_pressed;
        }

        /**
         * Whether the middle mouse button is pressed at the moment
         *
         * @return bool
         */
        bool input_handler::is_middle_mouse_button_pressed() const {
            return m_middle_mouse_button_pressed;
        }

        /**
         * Whether the left mouse button is pressed at the moment
         *
         * @return bool
         */
        bool input_handler::is_left_mouse_button_pressed() const {
            return m_left_mouse_button_pressed;
        }

        /**
         * Get the current mouse position
         *
         * @return vec2_t - with the x and y coordinates
         */
        math::vec2_t *input_handler::get_mouse_position() const {
            return m_mouse_position;
        }

        /**
         * Whether a key is down at the moment
         *
         * @param key
         * @return
         */
        bool input_handler::is_key_down(SDL_Scancode key) const {
            if (m_key_states != nullptr) {
                return m_key_states[key] == 1;
            }

            return false;
        }

        void input_handler::update(const SDL_Event &event) {
            // Update the stats
            if (update_states(event)) {
                // Call the observers whenever the state is changed
                notify_observers(this, "");
            }
        }

        /**
         * Update the states with the given SDL_Event
         *
         * @param event
         * @return bool - whether the state is changed or not
         */
        bool input_handler::update_states(const SDL_Event &event) {
            auto &eventbus = eventbus::eventbus::get_instance();
            switch (event.type) {
                case SDL_MOUSEBUTTONDOWN:
                    // Fire a mouse button down event
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        m_left_mouse_button_pressed = true;
                        events::mouse_button_down<input::mouse_buttons::LEFT> mouse_button_down_event;
                        eventbus.fire(mouse_button_down_event);
                    } else if (event.button.button == SDL_BUTTON_MIDDLE) {
                        m_middle_mouse_button_pressed = true;
                        events::mouse_button_down<input::mouse_buttons::MIDDLE> mouse_button_down_event;
                        eventbus.fire(mouse_button_down_event);
                    } else if (event.button.button == SDL_BUTTON_RIGHT) {
                        m_right_mouse_button_pressed = true;
                        events::mouse_button_down<input::mouse_buttons::RIGHT> mouse_button_down_event;
                        eventbus.fire(mouse_button_down_event);
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    // Fire a mouse button up event
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        m_left_mouse_button_pressed = false;
                        events::mouse_button_up<input::mouse_buttons::LEFT> mouse_button_up_event;
                        eventbus.fire(mouse_button_up_event);
                    } else if (event.button.button == SDL_BUTTON_MIDDLE) {
                        m_middle_mouse_button_pressed = false;
                        events::mouse_button_up<input::mouse_buttons::MIDDLE> mouse_button_up_event;
                        eventbus.fire(mouse_button_up_event);
                    } else if (event.button.button == SDL_BUTTON_RIGHT) {
                        m_right_mouse_button_pressed = false;
                        events::mouse_button_up<input::mouse_buttons::RIGHT> mouse_button_up_event;
                        eventbus.fire(mouse_button_up_event);
                    }
                    break;
                case SDL_MOUSEMOTION: {
                    // Fire a mouse motion event
                    m_mouse_position->x = (float)event.motion.x;
                    m_mouse_position->y = (float)event.motion.y;
                    events::mouse_motion mouse_motion_event(*m_mouse_position);
                    eventbus.fire(mouse_motion_event);
                    }
                    break;
                case SDL_KEYDOWN: {
                    // Fire a key down event
                    events::key_down key_down_event(keycodes::sdl_scancode_to_keycode(event.key.keysym.scancode));
                    eventbus.fire(key_down_event);
                    m_key_states = SDL_GetKeyboardState(0);
                    }
                    break;
                case SDL_KEYUP:
                    m_key_states = SDL_GetKeyboardState(0);
                    break;
                default:
                    return false;
            }

            return true;
        }

        /**
         * Clean the input handler
         */
        void input_handler::clean() {
            // Nothing to do at the moment
        }
    }
}
