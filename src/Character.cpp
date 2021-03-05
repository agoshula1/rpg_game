#include <stdexcept>
#include <climits>
#include <rpg/Character.hpp>

//!@throw std::invalid_argument if name is empty or HP less than or equal to 0
Character::Character(std::string name, int hp, std::vector<std::string> moves) :
    name(name), max_hp(hp), curr_hp(hp), moves(moves) {
        if(name.empty()) throw std::invalid_argument("Name of character cannot be empty.");
        if(hp <= 0) throw std::invalid_argument("Character must be initially alive (HP greater than 0).");
    }

std::string Character::get_name() const{
    return name;
}

int Character::get_hp() const{
    return curr_hp;
}

std::vector<std::string> Character::get_moves() const{
    return moves;
}

bool Character::alive() const{
    return curr_hp > 0;
}

void Character::apply_hp_effect(int hp_effect){
    // do not change HP if already dead
    if(curr_hp > 0){
        curr_hp = hp_effect < 0 ? (curr_hp + hp_effect < 0 ? 0 : curr_hp + hp_effect) // avoid negative HP
            : (INT_MAX - hp_effect < curr_hp ? INT_MAX // avoid integer overflow
                : (curr_hp + hp_effect > max_hp ? max_hp : curr_hp + hp_effect)); // avoid higher than max HP
    }    
}

void Character::full_heal(){
    if(alive()) curr_hp = max_hp;
}

void Character::add_moves(std::vector<std::string> new_moves){
    moves.insert(moves.end(), new_moves.begin(), new_moves.end());
}