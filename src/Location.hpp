#include <string>
#include <vector>

#include "Character.hpp"

class Location{
    std::string name;
    std::string description;
    vector<Character> enemies;

    public:
        std::string name();
        std::string describe();
        vector<Character> enemies();
}