#include <string>
#include <vector>

#include "Character.hpp"

class Location{
    std::string name;
    std::string description;
    std::vector<Character> enemies;

    public:
        std::string get_name();
        std::string describe();
        std::vector<Character> get_enemies();
};