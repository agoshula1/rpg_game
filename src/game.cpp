#include <iostream>
#include <string>
#include <unordered_map>
#include <forward_list>
#include <vector>

#include "Move.hpp"

using um_move = std::unordered_map<std::string, Move>;
using um_enem = std::unordered_map<std::string, Character>;

void intro(){
    std::cout << "-------------------\n";
    std::cout << "      RPG Game     \n";
    std::cout << "-------------------\n\n";

    std::cout << "An introduction to the game.\n";
}

int exit(){
    std::cout << "See you later!\n";
    return 0;
}

void setup(um_move& moves, um_enem& enemies, std::forward_list<Location>& map){
    // define move types

    // player moves
    moves["Poke"] = Move("Poke", "a small jab at one target", -3, false);
    moves["Punch"] = Move("Punch", "a light punch to one target", -5, false);
    moves["Kick"] = Move("Kick", "a swift kick to one target", -7, false);
    moves["Knockdown"] = Move("Knockdown", "a forcefull hit to one target", -10, false);
    
    // moves corresponding to inventory items
    moves["Medicine"] = Item("Medicine", "heals one target by 10 HP", 10, false, 0);
    moves["Bomb"] = Item("Bomb", "causes explosion hitting all enemies", -10, true, 0);
    
    // enemy/ally moves
    moves[""]

    // define enemy types

    enemies["easy"] = Character("easy", 10, {});
    

    // add each location
    map.insert_after(map.before_begin(), Location());
}

void hp_status(Character& prot, Character& ally, vector<Character>& enemies){
    std::cout << "\n---------\n";
    std::cout << "HP Status\n";
    std::cout << "---------\n";
    std::cout << prot.name() << " - " << prot.hp() << "\n";
    if(ally){
        std::cout << ally.name() << " - " << ally.hp() << "\n";
    }
    for(auto enemy : enemies){
        std::cout << enemy.name() << " - " << enemy.hp() << "\n";
    }
}

bool battle(Character& prot, Character& ally, vector<std::string>& enem_names, um_enem& enemy_types){
    // instantiate enemies in location
    vector<Character> enemies;
    for(auto enem_name : enem_names){
        enemies.push_back(enemy_types[enem_name]);
    }

    // announce battle if enemies exist
    if(!enemies.empty()){
        std::cout << "\n===================\n";
        std::cout << "A battle has begun!\n";
        std::cout << "===================\n";
    }

    hp_status(prot, ally, enemies);
    while(battle_ongoing(prot, ally, enemies)){
        next_user_move(prot, ally, enemies); // main character goes first

        hp_status(prot, ally, enemies);
        if(!battle_ongoing(prot, ally, enemies)) break;

        next_user_move(ally, prot, enemies); // ally goes next (if exists)
        hp_status(prot, ally, enemies);
        if(!battle_ongoing(prot, ally, enemies)) break;

        next_enemy_moves(enemies, ally, prot); // then enemies
        hp_status(prot, ally, enemies);
    }

    return prot.hp() > 0 && (ally ? ally.hp() : 1) > 0; // determine if main character and ally are still alive
}

int main(){
    intro();
    
    std::cout << "Are you ready to start (enter y for 'yes')?\n";
    if(to_lower(std::cin) != 'y'){
        // exit game
        return exit();
    }

    // game setup
    um_move move_types;
    um_enem enemy_types;
    std::forward_list<Location> map;
    setup(move_types, enemy_types, map);  

    // init status
    Character prot = new Character("hero", 15, {"Poke", "Medicine", "Bomb"});
    Character ally = NULL;
    vector<Character> party = {};
    
    // play game
    int hp_regain;
    vector<std::string> enemies; // names of enemies in current location
    int choice;
    bool won = true;
    for(Location& loc : map){
        std::cout << loc.describe() << "\n";
        
        // apply any healing effect from current location
        hp_regain = prot.hp();
        prot.heal(loc.heal_effect());
        hp_regain = prot.hp() - hp_regain;
        if(hp_regain > 0){
            std::cout << "You regained " << hp_regain << " HP!\n";
        }

        // battle any enemies in area
        enemies = loc.enemies();
        if(!enemies.empty()){
            won = battle(prot, ally, loc.enemies(), enemy_types)
            if(!won){
                std::cout << "Game Over.\n";
                break; // end game
            } else {
                std::cout << "You won the battle!\n";
            }
        }

        // check for locations with special events
        if(loc.name().substr(3) == "Inn"){
            // replenish supplies while at the inn
            move_types["Medicine"].replenish(3);
            std::cout << "You picked up 3 medicines at the inn (for a total of " << move_types["Medicine"].uses() << ")\n";
            move_types["Bomb"].replenish(2);
            std::cout << "You picked up 2 bombs at the inn (for a total of " << move_types["Bomb"].uses() << ")\n";
        } else if(loc.name().substr(4) == "Boss"){
            // enemy becomes a new ally
            Character new_ally = enemy_types[enemies[0]];
            new_ally.add_moves({"Medicine", "Bomb"});
            
            std::cout << new_ally.name() << " became a new ally!\n";
            if(!ally){
                ally = new_ally;
            } else {
                std::cout << "Switch your current ally with " << new_ally.name() << " (enter y for 'yes)?";
                if(to_lower(std::cin) != 'y'){
                    // switchout allys
                    party.push_back(ally);
                    ally = new_ally;
                } else {
                    party.push_back(new_ally);
                }
            }
        }

        // user chooses next step
        while(choice != 1 || choice != 2){
            std::cout << "What would you like to do next?\n";
            std::cout << "1) Move forward\n";
            std::cout << "2) Quit game\n";
            std::cin >> choice;
        }
        if(choice == 2){
            std::cout << "Are you sure you would like to quit? All progress will be lost.\n";
            std::cout << "(Enter y for 'yes')\n";
            if(tolower(std::cin) == 'y'){
                return exit();
            }
        }
        choice = 0; // reset
    }

    if(won){
        // win sequence
        std::cout << "Congratulations! You won the game!\n";
    }
}