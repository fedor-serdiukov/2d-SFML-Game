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

void Ally::serialize(std::ostream& ofs) const {
    ofs.write(reinterpret_cast<const char*>(&health), sizeof(health));
    ofs.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
}

void Ally::deserialize(std::istream& ifs) {
    ifs.read(reinterpret_cast<char*>(&health), sizeof(health));
    ifs.read(reinterpret_cast<char*>(&damage), sizeof(damage));
}