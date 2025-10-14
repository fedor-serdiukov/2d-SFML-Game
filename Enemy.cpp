#include "Enemy.h"

Enemy::Enemy(int health, int damage) : health(health), damage(damage) {}

int Enemy::get_health() const {
    return health;
}

int Enemy::get_damage() const {
    return damage;
}

void Enemy::change_health(int delta) {
    health += delta;
    if (health < 0) health = 0;
}