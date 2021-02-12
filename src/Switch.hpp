#ifndef SWITCH_H
#define SWITCH_H

#include "Move.hpp"

class Switch : public Move{
    public:
        Switch(std::string, std::string);
        void use(Character&, Character&);
};

#endif