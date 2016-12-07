//
// Created by Mark on 21-11-2016.
//


#include <random>
#include "enemy.h"
namespace domain {
    namespace nations {
        enemy::enemy(std::string _name, int _oppertunitycosts, bool _boss) {
            name = _name;
            oppertunitycosts =_oppertunitycosts;
            boss = _boss;
        }

        enemy::enemy(std::string _name, int _mindamage, int _maxdamage, double _attackspersecond, int _hitpoints, int _grantedXP, int _range, int _movement, bool _boss, std::shared_ptr<nation> _nation, int _oppertunitycosts) : m_destination(nullptr) {
            name = _name;
            mindamage = _mindamage;
            maxdamage = _maxdamage;
            attackspersecond = _attackspersecond;
            hitpoints = _hitpoints;
            grantedXP = _grantedXP;
            range = _range;
            movement = _movement;
            boss = _boss;
            Nation = _nation;
            oppertunitycosts =_oppertunitycosts;
        }

        std::string enemy::getName() {
            return Nation.get()->getprefixname()+" - "+name;


        }


        int enemy::getDamage() {
            std::random_device rd; // obtain a random number from hardware
            std::mt19937 rnd(rd()); // seed the generator
            std::uniform_int_distribution<> damage(mindamage, maxdamage);
            return damage(rnd);
        }

        double enemy::getattackspersecond() {
            return attackspersecond;
        }

        int enemy::getHitpoints(){
            return hitpoints;
        }

        int enemy::getRange(){
            return range;
        }
        int enemy::getMovement(){
            return movement;
        }
        int enemy::getOppertunity()const {
            return oppertunitycosts;
        }

        bool enemy::getBoss(){
            return boss;
        }

        int  enemy::lowerHitpoints(int points){

            if(hitpoints <= points){
                delete this;
                return grantedXP;
            }
            else{
                hitpoints = hitpoints - points;
            }
            return 0;
        }

        enemy::~enemy() {

        }

        engine::math::box2_t enemy::get_box() const {
            return *m_destination;
        }

        void enemy::set_box(std::shared_ptr<engine::math::box2_t> destination) {
            m_destination = destination;
        }

        std::shared_ptr<domain::map::field> enemy::update(std::shared_ptr<domain::map::map> current_map, unsigned int elapsed_time) {
            set_ai(current_map);
            auto location = m_ai.move(elapsed_time);
            return location;
        }

        void enemy::set_ai(std::shared_ptr<domain::map::map> current_map) {
            if(!m_ai.is_initialised())
            {
                m_ai = domain::map::ai(current_map);
            }
        }

        bool operator<(const std::shared_ptr<enemy>&  s1, const std::shared_ptr<enemy>&  s2){

            return s1->getOppertunity() < s2->getOppertunity();
        }

    }
}
