#include "Character.hpp"

Character::Character(std::string name, int hp, std::vector<std::string> moves) :
    name(name), max_hp(hp), curr_hp(hp), moves(moves) {}

std::string Character::get_name(){
    return name;
}

int Character::get_hp(){
    return curr_hp;
}

std::vector<std::string> Character::get_moves(){
    return moves;
}

bool Character::alive(){
    return curr_hp > 0;
}

void Character::heal(int hp){
    curr_hp + hp > max_hp ? max_hp : curr_hp + hp; // heal up to max HP
}

void Character::full_heal(){
    curr_hp = max_hp;
}

void Character::take_damage(int damage){
    curr_hp + damage;
}

void Character::add_moves(std::vector<std::string> new_moves){
    moves.insert(moves.end(), new_moves.begin(), new_moves.end());
}
