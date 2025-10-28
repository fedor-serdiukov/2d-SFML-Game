#include "Ally.h"

Ally::Ally(int hp, int dmg) : health(hp), damage(dmg) {}

Ally::Ally(const Ally& other)
    : health(other.health), damage(other.damage) {}

int Ally::getHealth() const {
    return health;
}

int Ally::getDamage() const {
    return damage;
}

void Ally::changeHealth(int amount) {
    health += amount;
    if (health < 0) health = 0;
}