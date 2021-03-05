#include <stdexcept>
#include <rpg/Move.hpp>

Move::Move(std::string name, std::string description, int hp_effect, bool target_all)
    : name(name), description(description), hp_effect(hp_effect), target_all(target_all) {
        if(name.empty()) throw std::invalid_argument("Name of move cannot be empty.");
        if(description.empty()) throw std::invalid_argument("Description of move cannot be empty.");
    }
    
std::string Move::get_name() const{
    return name;
}

std::string Move::describe() const{
    return description;
}

int Move::get_hp_effect() const{
    return hp_effect;
}

bool Move::targets_all() const{
    return target_all;
}

void Move::use(std::vector<Character>& targets){
    for(Character& target : targets){
        target.apply_hp_effect(hp_effect);
    }
}