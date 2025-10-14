#include "Player.h"
#include <iostream>

Player::Player(int max_health, int melee_damage, int ranged_damage)
    : health(max_health), max_health(max_health),
      melee_damage(melee_damage), ranged_damage(ranged_damage),
      combat_mode(CombatMode::Melee), switching_mode(false), slowed_turns(0) {}

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
    } else {
        combat_mode = CombatMode::Melee;
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
}

bool Player::is_slowed() const {
    return slowed_turns > 0;
}

void Player::apply_slow(int turns) {
    slowed_turns = turns;
}

void Player::decrement_slow() {
    if (slowed_turns > 0) --slowed_turns;
}