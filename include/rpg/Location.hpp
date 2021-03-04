#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include <vector>

#include "Character.hpp"

class Location{
    std::string name;
    std::string description;
    std::vector<Character> enemies;

    public:
        Location(std::string, std::string, std::vector<Character>);
        std::string get_name();
        std::string describe();
        std::vector<Character> get_enemies();
};

#endif