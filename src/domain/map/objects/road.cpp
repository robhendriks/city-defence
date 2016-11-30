//
// Created by te on 14-Nov-16.
//

#include "road.h"

namespace domain {
    namespace map {
        namespace objects {

            road::road(engine::math::box2_t box, type type1) : dragable_field_object(box), m_type(type1) {}

            road::road(std::shared_ptr<field> field1, type type1) : dragable_field_object(field1), m_type(type1) {}

            /**
             * Copy constructor
             */
            road::road(const road &obj) : dragable_field_object(obj) {
                m_type = obj.get_type();
            }

            type road::get_type() const {
                return m_type;
            }

            dragable_field_object *road::clone() const {
                return new road(*this);
            }
        }
    }
}
