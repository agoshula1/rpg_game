#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>

class Character{
    std::string name;
    int max_hp, curr_hp;
    std::vector<std::string> moves;

    public:
        Character(std::string, int, std::vector<std::string>);
        std::string get_name() const;
        int get_hp() const;
        std::vector<std::string> get_moves() const;
        bool alive() const;
        void full_heal();
        void apply_hp_effect(int);
        void add_moves(std::vector<std::string>);
};

#endif