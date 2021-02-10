#include "Move.hpp"

std::string Move::get_name(){
    return name;
}

std::string Move::describe(){
    return description;
}

int Move::get_hp_effect(){
    return hp_effect;
}

bool Move::targets_all(){
    return target_all;
}

void Move::use(std::vector<Character>& targets){
    for(Character& target : targets){
        target.apply_hp_effect(hp_effect);
    }
}