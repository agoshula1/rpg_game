#include <iostream>
#include <string>
#include <unordered_map>
#include <forward_list>
#include <vector>

#include "Move.hpp"

class Game{
    // game features
    std::unordered_map<std::string, Move> move_types;
    // player moves
    move_types["Poke"] = Move("Poke", "a small jab at one target", -3, false);
    move_types["Punch"] = Move("Punch", "a light punch to one target", -5, false);
    move_types["Kick"] = Move("Kick", "a swift kick to one target", -7, false);
    move_types["Knockdown"] = Move("Knockdown", "a forcefull hit to one target", -10, false);
    
    // moves corresponding to inventory items
    move_types["Medicine"] = Item("Medicine", "heals one target by 10 HP", 10, false, 0);
    move_types["Bomb"] = Item("Bomb", "causes explosion hitting all enemies", -10, true, 0);

    // move corresponding to switching out allies
    move_types["Switch Ally"] = Switch("Switch Ally", "switches ally out with another party member", 0, false);
    
    // enemy/ally moves
    move_types[""]

    std::unordered_map<std::string, Character> enemy_types;
    enemy_types["easy"] = Character("easy", 10, {});

    std::forward_list<Location> map;
    map.insert_after(map.before_begin(), Location());

    // init status
    Character prot = new Character("hero", 15, {"Poke", "Medicine", "Bomb"});
    Character ally = NULL;
    vector<Character> party = {};

    void intro(){
        std::cout << "-------------------\n";
        std::cout << "      RPG Game     \n";
        std::cout << "-------------------\n\n";

        std::cout << "An introduction to the game.\n";
    }

    void hp_status(vector<Character>& enemies){
        std::cout << "\n---------\n";
        std::cout << "HP Status\n";
        std::cout << "---------\n";
        std::cout << "1) " << prot.name() << " - " << prot.hp() << "\n";
        if(ally){
            std::cout << "2) " << ally.name() << " - " << ally.hp() << "\n";
        }
        for(int i = 0; i < enemies.size(); i++){
            std::cout << i+3 << ") " << enemy.name() << " - " << enemy.hp() << "\n";
        }
    }

    bool battle_ongoing(vector<Character>& enemies){
        if(!prot.alive()) return false;
        if(ally && !ally.alive()) return false;
        for(auto enemy : enemies){
            if(enemy.alive()) return true;
        }
        return false; // no undefeated enemies left
    }

    void next_user_move(Character& hero1, Character& hero2, vector<Character>& enemies){
        int move_num, command_num, target_num = 0;
        const vector<std::string>& moves = hero1.moves();
        vector<Character> targets;
        
        std::cout << hero1.name() << "'s turn.";

        while(command_num != 2){
            // no valid move chosen to be used yet
            std::cout << "Choose one of the following moves:\n";
            for(int i = 0; i < moves.size(); ++i){
                std::cout << i+1 << ") " << moves[i] << "\n";
            }
            std::cin >> move_num;

            if(move_num < 1 || move_num > moves.size()) continue; // invalid choice

            std::cout << "Would you like:\n";
            std::cout << "1) information about this move\n";
            std::cout << "2) to use this move\n";
            std::cin >> command_num;

            if(command_num == 1){
                std::cout << moves[move_num-1] << ": " << move_types[moves[move_num-1]].describe() << "\n\n";
            }
        }

        if(moves[move_num-1] == 'Switch Ally'){
            targets = party; // switching with another ally in party
        } else if(move_types[moves[move_num-1]].hp_effect() > 0){
            targets = {hero1, hero2}; // healing self and/or teamate
        } else {
            // attacking one or all undefeated enemies
            for(Character enemy : enemies){
                if(enemy.alive()) targets.push_back(enemy);
            }
        }

        if(!move_types[moves[move_num-1]].target_all()){
            // get specific target from user
            while(target_num < 1 || target_num > targets.size()){
                std::cout << "Who would you like to target:\n";
                for(int i = 0; i < targets.size(); ++i){
                    std::cout << i+1 << ") " << targets[i].name() << "\n";
                }
                std::cout >> target_num
            }
            targets = {targets[target_num-1]};
        }

        // perform move
        move_types[moves[move_num-1]].perform(targets);
    }

    bool battle(vector<std::string>& enem_names){
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

        hp_status(enemies);
        while(battle_ongoing(enemies)){
            next_user_move(prot, ally, enemies); // main character goes first

            hp_status(enemies);
            if(!battle_ongoing(enemies)) break;

            next_user_move(ally, prot, enemies); // ally goes next (if exists)
            hp_status(enemies);
            if(!battle_ongoing(enemies)) break;

            next_enemy_moves(enemies, ally, prot); // then enemies
            hp_status(enemies);
        }

        return prot.alive() && ally ? ally.alive() : true; // determine if main character and ally are still alive
    }

    void play(){
        intro();
        
        std::cout << "Are you ready to start (enter y for 'yes')?\n";
        if(to_lower(std::cin) != 'y'){
            // exit game
            std::cout << "See you later!\n";
            return;
        }    
        
        // play game
        int hp_regain;
        vector<std::string> enemies; // names of enemies in current location
        int choice;
        bool won = true;
        for(Location& loc : map){
            std::cout << loc.describe() << "\n";
            
            // apply any healing effect from current location
            // hp_regain = prot.hp();
            // prot.heal(loc.heal_effect());
            // hp_regain = prot.hp() - hp_regain;
            // if(hp_regain > 0){
            //     std::cout << "You regained " << hp_regain << " HP!\n";
            // }

            // battle any enemies in area
            enemies = loc.enemies();
            if(!enemies.empty()){
                won = battle(prot, ally, loc.enemies(), enemy_types);
                if(!won){
                    std::cout << "Game Over.\n";
                    break; // end game
                } else {
                    std::cout << "You won the battle!\n";
                }
            }

            // check for locations with special events
            if(loc.name().substr(3) == "Inn"){
                if(loc != map.front()){
                    // fully heal party
                    prot.full_heal();
                    ally.full_heal();
                    for(Character& member : party){
                        member.full_heal();
                    }
                    std::cout << "You and your party are fully healed!\n";
                }
                
                // replenish supplies while at the inn
                move_types["Medicine"].replenish(3);
                std::cout << "You picked up 3 medicines at the inn (for a total of " << move_types["Medicine"].uses() << ")\n";
                move_types["Bomb"].replenish(2);
                std::cout << "You picked up 2 bombs at the inn (for a total of " << move_types["Bomb"].uses() << ")\n";
            } else if(loc.name().substr(4) == "Boss"){
                // enemy becomes a new ally
                Character new_ally = enemy_types[enemies[0]];
                new_ally.add_moves({"Medicine", "Bomb"});
                // add to party
                party.push_back(new_ally);
                
                std::cout << new_ally.name() << " became a new ally!\n";
                if(!ally){
                    // move from party to current ally
                    ally = new_ally;
                    party.pop_back();
                } else {
                    std::cout << "Switch your current ally with " << new_ally.name() << " (enter y for 'yes')?";
                    if(to_lower(std::cin) != 'y'){
                        // switchout allies
                        move_types["Switch"].switch(ally, party, party.end()-1);
                    }
                    if(party.size() == 1){
                        // add new move to allow user to switch allies
                        prot.add_moves({"Switch Ally"});
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
                    // exit game
                    std::cout << "See you later!\n";
                    return;
                }
            }
            choice = 0; // reset
        }

        if(won){
            // win sequence
            std::cout << "Congratulations! You won the game!\n";
        }
    }
}

int main(){
    Game session = new Game();
    session.play();
}

