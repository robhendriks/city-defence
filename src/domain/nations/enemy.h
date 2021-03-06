//
// Created by Mark on 21-11-2016.
//

#ifndef CITY_DEFENCE_ENEMY_H
#define CITY_DEFENCE_ENEMY_H

#include <string>
#include <memory>
#include "nation.h"
#include "../drawable/drawable_game_object.h"
#include "../map/ai/ai.h"
#include "../combat/attacker.h"
#include "../combat/defender.h"

namespace domain {
    namespace nations {
        class enemy : public domain::combat::attacker, public domain::combat::defender {
        public:
            enemy(std::string name, int min_damage, int max_damage,
                  double attacks_per_second, int hitpoints, int granted_xp, int range,
                  int movement, bool boss, nation &nation1, int oppertunity_costs);

            // Copy constructor
            enemy(const enemy& other);

            void set_nation(nation &nation1);

            void dispose();

            //Returns nationname + unitname
            std::string get_name();

            std::string get_name_without_prefix() const;

            int get_oppertunity_cost() const;

            bool is_boss() const;

            bool is_disposed() const;

            virtual void set_box(engine::math::box2_t destination);

            virtual engine::math::box2_t get_box() const;

            void update(unsigned int elapsed_time);

            bool is_box_set() const;

            enemy *clone() const;

            ~enemy();

        private:
            engine::math::box2_t m_destination;
            std::string m_name;
            //Cost for using this enemy; to generate proper waves.
            int m_oppertunity_cost;
            bool m_boss;
            bool m_disposed;
            nation *m_nation;
            bool m_box_isset;
        };

        bool operator<(const enemy &s1, const enemy &s2);
    }
}

#endif //CITY_DEFENCE_ENEMY_H
