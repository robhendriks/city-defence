//
// Created by te on 08-Dec-16.
//

#ifndef CITY_DEFENCE_MOVE_STATE_H
#define CITY_DEFENCE_MOVE_STATE_H

#include "state.h"
#include "../../field.h"

namespace domain {
    namespace map {
        namespace ai {
            namespace states {
                class move_state : public state {
                public:
                    move_state();
                    void update(ai * ai, unsigned int elapsed_time);
                private:
                    int m_time_moved_on_current_field = -1;
                    // to make sure you don't go back (maybe we want him to go back if building is placed behind him/
                    // need to experiment
                    std::shared_ptr<field> m_last_field;
                    std::shared_ptr<field> m_next_field;

                    // to save the initial state and what to add to it so we don't need to recalculate it
                    engine::math::box2_t m_init_unit_box = {{0,0},{0,0}};
                    engine::math::box2_t m_difference_box = {{0,0},{0,0}};

                    void calculate_initial_state_with_difference(domain::map::ai::ai *ai);

                    void move(ai * ai, unsigned int elapsed_time);
                    std::shared_ptr<field> get_next_field(domain::map::ai::ai *ai);
                };
            }
        }
    }
}

#endif //CITY_DEFENCE_MOVE_STATE_H