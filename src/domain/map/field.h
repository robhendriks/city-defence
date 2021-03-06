#ifndef CITY_DEFENCE_BASE_FIELD_H
#define CITY_DEFENCE_BASE_FIELD_H

#include "../../engine/graphics/texture_manager.h"
#include "../../engine/observer/observee.h"
#include "map.h"
#include "objects/field_object.h"
#include "../drawable/drawable_game_object.h"
#include "../../engine/draganddrop/dropable.h"

// Forward declarations
namespace domain {
    namespace map {
        namespace objects {
            class field_object;
        }
    }
}

namespace domain {
    namespace map {
        class map;
    }
}

namespace domain {
    namespace map {
        class field : public drawable::drawable_game_object, public engine::draganddrop::dropable,
                      public engine::observer::observee<field> {
        public:
            const static unsigned int FLAG_NONE = 0x00;
            const static unsigned int FLAG_WEIGHT = 0x01;
            const static unsigned int FLAG_TARGET = 0x02;

            field(map &map1, engine::math::vec2_t pos);

            ~field();

            void draw(drawable::draw_managers_wrapper &draw_managers, unsigned int time_elapsed);

            engine::math::box2_t get_box() const;

            bool drop(engine::draganddrop::dragable* dragable1);

            void set_box(engine::math::box2_t box);

            bool place_object(objects::field_object* object);

            bool has_object() const;

            objects::field_object *get_object() const;

            std::vector<field*> get_neighbors() const;

            engine::math::vec2_t get_position() const;

            long get_weight() const;

            void set_weight(long weight);

            unsigned int get_flags() const;

            void set_flags(unsigned int flags);

        private:
            unsigned int m_flags;
            map &m_map;
            engine::math::vec2_t m_pos;
            objects::field_object *m_object;
            std::unique_ptr<engine::math::box2_t> m_box;
            long m_weight = 0;
        };
    }
}
#endif //CITY_DEFENCE_BASE_FIELD_H
