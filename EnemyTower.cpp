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

void EnemyTower::serialize(std::ostream& ofs) const {
    ofs.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
    ofs.write(reinterpret_cast<const char*>(&range), sizeof(range));
    ofs.write(reinterpret_cast<const char*>(&cooldown), sizeof(cooldown));
    ofs.write(reinterpret_cast<const char*>(&turns_until_ready), sizeof(turns_until_ready));
}
void EnemyTower::deserialize(std::istream& ifs) {
    ifs.read(reinterpret_cast<char*>(&damage), sizeof(damage));
    ifs.read(reinterpret_cast<char*>(&range), sizeof(range));
    ifs.read(reinterpret_cast<char*>(&cooldown), sizeof(cooldown));
    ifs.read(reinterpret_cast<char*>(&turns_until_ready), sizeof(turns_until_ready));
}