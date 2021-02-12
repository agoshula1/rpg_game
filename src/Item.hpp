#ifndef ITEM_H
#define ITEM_H

#include "Move.hpp"

class Item : public Move{
    int uses;

    public:
        Item(std::string, std::string, int, bool, int);
        std::string describe(); // need to report # of uses left
        void replenish(int);
        void use(std::vector<Character>&); // need to decrement uses
};

#endif