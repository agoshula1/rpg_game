#include <string>
#include <vector>

class Move{
    std::string name;
    std::string description;
    int hp_effect;
    bool target_all;
    
    public:
        Move(std::string, std::string, int, bool);
};

class Item : public Move{
    int uses;

    public:
        Item(int num_uses);
        void use();
};