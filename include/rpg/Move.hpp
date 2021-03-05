#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <vector>

#include "Character.hpp"

class Move{
    protected:
        std::string name;
        std::string description;
        int hp_effect;
        bool target_all;
    
    public:
        Move(std::string, std::string, int, bool);
        std::string get_name() const;
        std::string describe() const;
        int get_hp_effect() const;
        bool targets_all() const;
        void use(std::vector<Character>&);
};

#endif