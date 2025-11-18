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

void Enemy::serialize(std::ostream& ofs) const {
    ofs.write(reinterpret_cast<const char*>(&health), sizeof(health));
    ofs.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
}
void Enemy::deserialize(std::istream& ifs) {
    ifs.read(reinterpret_cast<char*>(&health), sizeof(health));
    ifs.read(reinterpret_cast<char*>(&damage), sizeof(damage));
}