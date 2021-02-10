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
        std::string get_name();
        std::string describe();
        int get_hp_effect();
        bool targets_all();
        void use(std::vector<Character>&);
};