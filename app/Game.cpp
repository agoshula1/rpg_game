#include <iostream>
#include <string>
#include <unordered_map>
#include <forward_list>
#include <vector>
#include <ctime>

#include <rpg/Character.hpp>
#include <rpg/Move.hpp>
#include <rpg/Item.hpp>
#include <rpg/Switch.hpp>

class Game{
    // game features
    std::unordered_map<std::string, Move> move_types;
    Character prot, ally;
    std::vector<Character> party;

    Game(){
        // player moves
        move_types["Poke"] = Move("Poke", "A small jab at one target.", -3, false);
        move_types["Punch"] = Move("Punch", "A light punch to one target.", -5, false);
        move_types["Kick"] = Move("Kick", "A swift kick to one target.", -7, false);
        move_types["Knockdown"] = Move("Knockdown", "A forcefull hit to one target.", -10, false);
        
        // moves corresponding to inventory items
        move_types["Use Medicine"] = Item("Use Medicine", "Heals one target by 10 HP.", 10, false, 0);
        move_types["Use Bomb"] = Item("Use Bomb", "Causes explosion hitting all enemies.", -10, true, 0);

        // move corresponding to switching out allies
        move_types["Switch Ally"] = Switch("Switch Ally", "Switches ally out with another party member.");
        
        // enemy/ally moves
        // move_types[""]

        std::unordered_map<std::string, Character> enemy_types;
        enemy_types["easy"] = Character("easy", 10, {});

        std::forward_list<Location> map;
        // map.insert_after(map.before_begin(), Location());

        // init status
        prot = new Character("hero", 30, {"Poke", "Use Medicine", "Use Bomb"});
        ally = NULL;
        party = {};
    }

    void intro(){
        std::cout << "-------------------\n";
        std::cout << "      RPG Game     \n";
        std::cout << "-------------------\n\n";

        std::cout << "An introduction to the game.\n";
    }

    void hp_status(std::vector<Character>& enemies){
        std::cout << "\n---------\n";
        std::cout << "HP Status\n";
        std::cout << "---------\n";
        std::cout << "1) " << prot.get_name() << " - " << prot.get_hp() << "\n";
        if(ally != NULL){
            std::cout << "2) " << ally.get_name() << " - " << ally.get_hp() << "\n";
        }
        for(int i = 0; i < enemies.size(); i++){
            if(enemies[i].alive()){
                std::cout << i+3 << ") " << enemies[i].get_name() << " - " << enemies[i].get_hp() << "\n";
            }
        }
    }

    bool battle_ongoing(std::vector<Character>& enemies){
        if(!prot.alive()) return false;
        if(ally != NULL && !ally.alive()) return false;
        for(auto enemy : enemies){
            if(enemy.alive()) return true;
        }
        return false; // no undefeated enemies left
    }

    void next_user_move(Character& hero1, Character& hero2, std::vector<Character>& enemies){
        int move_num, command_num, target_num;
        const std::vector<std::string>& moves = hero1.get_moves();
        std::vector<Character> targets;
        
        std::cout << hero1.get_name() << "'s turn.";

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

        if(moves[move_num-1] == "Switch Ally"){
            targets = party; // switching with another ally in party
        } else if(move_types[moves[move_num-1]].get_hp_effect() > 0){
            targets = {hero1, hero2}; // healing self and/or teamate
        } else {
            // attacking one or all undefeated enemies
            for(Character enemy : enemies){
                if(enemy.alive()) targets.push_back(enemy);
            }
        }

        if(!move_types[moves[move_num-1]].targets_all()){
            // get specific target from user
            while(target_num < 1 || target_num > targets.size()){
                std::cout << "Who would you like to target:\n";
                for(int i = 0; i < targets.size(); ++i){
                    std::cout << i+1 << ") " << targets[i].get_name() << "\n";
                }
                std::cin >> target_num;
            }
            targets = {targets[target_num-1]};
        }

        // perform move
        std::cout << hero1.get_name() << " uses " << moves[move_num-1] << ".\n";
        if(moves[move_num-1] == "Switch Ally"){
            move_types[moves[move_num-1]].use(ally, targets[0]);
        } else {
            move_types[moves[move_num-1]].use(targets);
        }
    }

    void next_enemy_moves(std::vector<Character>& enemies){
        std::string move;
        std::vector<Character> targets;
        for(Character& enemy : enemies){
            if(!enemy.alive()) continue;

            // pick move at random
            move = enemy.get_moves()[rand() % enemy.get_moves().size()];

            if(move_types[move].get_hp_effect() > 0){
                targets = enemies; // healing self and/or teamates
            } else {
                targets = {prot, ally}; // attacking hero and/or ally
            }

            if(!move_types[move].targets_all()){
                // pick specific target at random
                targets = {targets[rand() % targets.size()]};
            }

            // perform move
            std::cout << enemy.get_name() << " uses " << move << ".\n";
            move_types[move].use(targets);
        }
    }

    bool battle(std::vector<Character>& enemies){
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

            next_enemy_moves(enemies); // then enemies
            hp_status(enemies);
        }

        return prot.alive() && ally ? ally.alive() : true; // determine if main character and ally are still alive
    }

    public:
        void play(){
        intro();
        
        std::cout << "Are you ready to start (enter y for 'yes')?\n";
        if(to_lower(std::cin) != 'y'){
            // exit game
            std::cout << "See you later!\n";
            return;
        }    
        
        // play game
        std::vector<Character> enemies;
        int choice;
        bool won = true;
        for(Location& loc : map){
            std::cout << loc.describe() << "\n";
            
            // battle any enemies in area
            enemies = loc.get_enemies();
            if(!enemies.empty()){
                won = battle(enemies);
                if(!won){
                    std::cout << "Game Over.\n";
                    break; // end game
                } else {
                    std::cout << "You won the battle!\n";
                }
            }

            // check for locations with special events
            if(loc.get_name().substr(3) == "Inn"){
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
                move_types["Use Medicine"].replenish(3);
                std::cout << "You picked up 3 medicines at the inn (for a total of " << move_types["Use Medicine"].uses() << ")\n";
                move_types["Use Bomb"].replenish(2);
                std::cout << "You picked up 2 bombs at the inn (for a total of " << move_types["Use Bomb"].uses() << ")\n";
            } else if(loc.get_name().substr(4) == "Boss"){
                // enemy becomes a new ally
                Character new_ally = enemies[0];
                new_ally.full_heal();
                new_ally.add_moves({"Use Medicine", "Use Bomb"});
                // add to party
                party.push_back(new_ally);
                
                std::cout << new_ally.get_name() << " became a new ally!\n";
                if(!ally){
                    // move from party to current ally
                    ally = new_ally;
                    party.pop_back();
                } else {
                    std::cout << "Switch your current ally with " << new_ally.get_name() << " (enter y for 'yes')?";
                    if(to_lower(std::cin) != 'y'){
                        // switchout allies
                        move_types["Switch Ally"].use(ally, party.back());
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
};

int main(int argc, char* argv[]){
    if (argc < 2) {
        // report version
        std::cout << argv[0] << " Version " << RPG_VERSION_MAJOR << "."
                << RPG_VERSION_MINOR << "\n";
        std::cout << "Usage: " << argv[0] << " number" << "\n";
        return 1;
    }
  
    // initialize random seed
    srand(time(NULL));

    Game session = new Game();
    session.play();
}

