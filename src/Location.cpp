#include <rpg/Location.hpp>

Location::Location(std::string name, std::string description, std::vector<Character> enemies)
    : name(name), description(description), enemies(enemies) {}

std::string Location::get_name(){
    return name;
}

std::string Location::describe(){
    return description;
}

std::vector<Character> Location::get_enemies(){
    return enemies;
}