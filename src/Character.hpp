#include <string>
#include <vector>

class Character{
    std::string name;
    int max_hp, curr_hp;
    std::vector<std::string> moves;

    public:
        Character(std::string, int, std::vector<std::string>);
        std::string get_name();
        int get_hp();
        std::vector<std::string> get_moves();
        bool alive();
        void heal(int);
        void full_heal();
        void take_damage(int);
        void add_moves(std::vector<std::string>);
};