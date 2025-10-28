#include "Player.h"
#include <iostream>
#include "SummonSpell.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"



bool Player::inputPlayerStats(int& player_max_health, int& melee_damage, int& ranged_damage) {
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
Player::Player(const Player& other)
    : health(other.health),
      max_health(other.max_health),
      melee_damage(other.melee_damage),
      ranged_damage(other.ranged_damage),
      score(other.score),
      combat_mode(other.combat_mode),
      switching_mode(other.switching_mode),
      slowed_turns(other.slowed_turns),
      hand(other.hand),
      killCount(other.killCount)
{}

Player::Player(int max_health, int melee_damage, int ranged_damage, size_t handSize)
    : health(max_health), max_health(max_health),
      melee_damage(melee_damage), ranged_damage(ranged_damage),
      combat_mode(CombatMode::Melee), switching_mode(false), slowed_turns(0),
      hand(handSize)
{
    std::cout << "Game started\nTo exit, press ESC\nMove with WASD, switch mode with SPACE\n";
    std::cout << "Press 1-3 to select spell, B to buy spell (50 points).\n";
}

void Player::set_max_health(int max_health) {
    this->max_health = max_health;
    this->health = max_health;
}

int Player::get_health() const {
    return health;
}

int Player::get_max_health() const {
    return max_health;
}

int Player::get_score() const {
    return score;
}

int Player::get_damage() const {
    return (combat_mode == CombatMode::Melee) ? melee_damage : ranged_damage;
}

CombatMode Player::get_combat_mode() const {
    return combat_mode;
}

void Player::toggle_combat_mode() {
    if (combat_mode == CombatMode::Melee) {
        combat_mode = CombatMode::Ranged;
        std::cout << "Switched to Ranged mode\n";
    } else {
        combat_mode = CombatMode::Melee;
        std::cout << "Switched to Melee mode\n";
    }
    switching_mode = true;

}

bool Player::is_switching_mode() const {
    return switching_mode;
}

void Player::reset_switching_mode() {
    switching_mode = false;
}

void Player::change_health(int delta) {
    health += delta;
    if (health > max_health) health = max_health;
    if (health < 0) health = 0;
}

void Player::change_score(int add_score) {
    score += add_score;
    if (score < 0) score = 0;
}

bool Player::is_slowed() const {
    return slowed_turns > 0;
}

void Player::apply_slow(int turns) {
    slowed_turns = turns;
}

void Player::decrement_slow() {
    if (slowed_turns > 0) {
        --slowed_turns;
        if (slowed_turns == 1) {
            std::cout << "Player is slowed and cannot move this turn!\n";
        }
    }
}

Hand& Player::getHand() {
    return hand;
}

void Player::incrementKillCount() {
    killCount++;
}

int Player::getKillCount() const {
    return killCount;
}

void Player::resetKillCount() {
    killCount = 0;
}

void Player::applyBuffToSpell(ISpell& spell) const {
    if (buffCharges == 0) return;

    DirectDamageSpell* dds = dynamic_cast<DirectDamageSpell*>(&spell);
    AreaDamageSpell* ads = dynamic_cast<AreaDamageSpell*>(&spell);
    TrapSpell* ts = dynamic_cast<TrapSpell*>(&spell);
    SummonSpell* ss = dynamic_cast<SummonSpell*>(&spell);

    if (dds) {
        dds->setRange(dds->getRange() + buffCharges);
        std::cout << "[BUFF] Прямой урон: Радиус увеличен на " << buffCharges << std::endl;
    } else if (ads) {
        ads->setRange(ads->getRange() + buffCharges);
        std::cout << "[BUFF] Урон по площади: Радиус увеличен на " << buffCharges << std::endl;
    } else if (ts) {
        ts->setDamage(ts->getDamage() + buffCharges * 10);
        std::cout << "[BUFF] Ловушка: Урон увеличен на " << buffCharges * 10 << std::endl;
    } else if (ss) {
        ss->setSummonsCount(ss->getSummonsCount() + buffCharges);
        std::cout << "[BUFF] Призыв: Количество союзников увеличено на " << buffCharges << std::endl;
    }
}