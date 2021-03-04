#include <rpg/Switch.hpp>

Switch::Switch(std::string name, std::string description) : Move(name, description, 0, false) {}

// hide Move::use (would have no effect)
void Switch::use(Character& ally_1, Character& ally_2){
    std::swap(ally_1, ally_2);
}

