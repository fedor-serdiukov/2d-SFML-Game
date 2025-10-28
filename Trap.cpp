#include "Trap.h"

Trap::Trap(int dmg) : damage(dmg) {}

int Trap::getDamage() const {
    return damage;
}