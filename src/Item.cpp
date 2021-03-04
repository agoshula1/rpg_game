#include <iostream>
#include <rpg/Item.hpp>

Item::Item(std::string name, std::string description, int hp_effect, bool target_all, int uses)
    : Move(name, description, hp_effect, target_all), uses(uses) {}

std::string Item::describe(){
    // include quantity
    return Move::describe() + " You have " + std::to_string(uses) + " left.";
}

void Item::replenish(int amount){
    uses += amount;
}

void Item::use(std::vector<Character>& targets){
    if(uses >= 1){
        Move::use(targets);
        --uses; // decrease supply
    } else {
        // no more available
        std::cout << "You are out of this item!\n";
    }
}