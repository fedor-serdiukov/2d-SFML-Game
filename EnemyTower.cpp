#include "EnemyTower.h"

EnemyTower::EnemyTower(int dmg, int rng, int cd)
    : damage(dmg), range(rng), cooldown(cd), turns_until_ready(0)
{}

void EnemyTower::tick() {
    if (turns_until_ready > 0) {
        --turns_until_ready;
    }
}

bool EnemyTower::isReady() const {
    return turns_until_ready == 0;
}

void EnemyTower::resetCooldown() {
    turns_until_ready = cooldown;
}

int EnemyTower::getRange() const {
    return range;
}

int EnemyTower::getDamage() const {
    return damage;
}