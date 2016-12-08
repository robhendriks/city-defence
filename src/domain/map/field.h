#ifndef CITY_DEFENCE_BASE_FIELD_H
#define CITY_DEFENCE_BASE_FIELD_H

#include "../../engine/graphics/texture_manager.h"
#include "../../engine/observer/observee.h"
#include "map.h"
#include "objects/field_object.h"
#include "../drawable/drawable_game_object.h"
#include "../../engine/draganddrop/dropable.h"

namespace domain {
    namespace map {
        namespace objects {
            class field_object;

            using field_object_ptr = std::shared_ptr<field_object>;
        }
    }
}

namespace domain {
    namespace map {
        class map;

        using map_ptr = std::shared_ptr<map>;
    }
}

namespace domain {
    namespace map {


        class field
            : public std::enable_shared_from_this<field>,
              public drawable::drawable_game_object,
              public engine::draganddrop::dropable,
              public engine::observer::observee<field> {
        public:

            field(const engine::math::vec2_t &pos);

            ~field();

            void draw(drawable::draw_managers_wrapper &draw_managers, unsigned int time_elapsed);

            engine::math::box2_t get_box() const;

            bool drop(engine::draganddrop::dragable *dragable1);

            void set_box(engine::math::box2_t box);

            bool place_object(const objects::field_object_ptr &object);

            bool has_object() const;

            objects::field_object_ptr get_object() const;

            std::vector<std::shared_ptr<field>> get_neighbors() const;

            engine::math::vec2_t get_position() const;

            map_ptr get_map() const;

            void set_map(const map_ptr &map);

            long get_weight() const;

            void set_weight(long weight);

        private:
            map_ptr m_map;
            engine::math::vec2_t m_pos;
            objects::field_object_ptr m_object;
            engine::math::box2_t *m_box;
            long m_weight = 0;
        };
    }
}
#endif //CITY_DEFENCE_BASE_FIELD_H
