#include <iostream>
#include "greeting_func.h"

bool inputPlayerStats(int& player_max_health, int& melee_damage, int& ranged_damage) {
    std::cout << "Welcome to the Game!\nFirst, you should choose player's health and damage from the list\n"
                 "| Health |\n"
                 "|   50   |\n"
                 "|   100  |\n"
                 "|   150  |" << std::endl;
    std::cin >> player_max_health;
    if (player_max_health != 50 && player_max_health != 100 && player_max_health != 150) {
        std::cout << "You chose something out of the list!" << std::endl;
        return false;
    }
    std::cout << "OK. Player's max health is " << player_max_health << ". Let's choose player's damage." << std::endl;
    std::cout << "| Damage |\n"
                 "|   15   |\n"
                 "|   25   |\n"
                 "|   35   |" << std::endl;
    std::cin >> melee_damage;
    if (melee_damage != 15 && melee_damage != 25 && melee_damage != 35) {
        std::cout << "You chose something out of the list!" << std::endl;
        return false;
    }
    std::cout << "OK. Player's damage is " << melee_damage << "." << std::endl;
    ranged_damage = melee_damage / 2 + 5;
    return true;
}