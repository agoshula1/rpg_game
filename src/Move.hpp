#include <string>
#include <vector>

#include "Character.hpp"

class Move{
    std::string name;
    std::string description;
    int hp_effect;
    bool target_all;
    
    public:
        Move(std::string, std::string, int, bool);
        std::string name();
        std::string describe();
        int hp_effect();
        bool target_all();
        void use(std::vector<Character>&);
};

class Item : public Move{
    int uses;

    public:
        Item(std::string, std::string, int, bool, int num_uses);
        std::string describe(); // need to report # of uses left
        void replenish(int);
        void use(std::vector<Character>&); // need to decrement uses
};

class Switch : public Move{
    public:
        Switch(std::string, std::string);
        void use(Character&, Character&);
}