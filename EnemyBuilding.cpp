#include "EnemyBuilding.h"

EnemyBuilding::EnemyBuilding(int interval, int e_health, int e_damage, int b_health)
    : spawn_interval(interval), turns_until_spawn(interval),
      enemy_health(e_health), enemy_damage(e_damage), health(b_health) {}

void EnemyBuilding::tick() {
    if (turns_until_spawn > 0) --turns_until_spawn;
}

bool EnemyBuilding::is_ready_to_spawn() const {
    return turns_until_spawn == 0;
}

void EnemyBuilding::reset_timer() {
    turns_until_spawn = spawn_interval;
}

int EnemyBuilding::get_enemy_health() const {
    return enemy_health;
}

int EnemyBuilding::get_enemy_damage() const {
    return enemy_damage;
}

int EnemyBuilding::get_spawn_interval() const {
    return spawn_interval;
}


int EnemyBuilding::get_health() const {
    return health;
}

void EnemyBuilding::change_health(int delta) {
    health += delta;
    if (health < 0) health = 0;
}

void EnemyBuilding::serialize(std::ostream& ofs) const {
    ofs.write(reinterpret_cast<const char*>(&spawn_interval), sizeof(spawn_interval));
    ofs.write(reinterpret_cast<const char*>(&turns_until_spawn), sizeof(turns_until_spawn));
    ofs.write(reinterpret_cast<const char*>(&enemy_health), sizeof(enemy_health));
    ofs.write(reinterpret_cast<const char*>(&enemy_damage), sizeof(enemy_damage));
    ofs.write(reinterpret_cast<const char*>(&health), sizeof(health));
}
void EnemyBuilding::deserialize(std::istream& ifs) {
    ifs.read(reinterpret_cast<char*>(&spawn_interval), sizeof(spawn_interval));
    ifs.read(reinterpret_cast<char*>(&turns_until_spawn), sizeof(turns_until_spawn));
    ifs.read(reinterpret_cast<char*>(&enemy_health), sizeof(enemy_health));
    ifs.read(reinterpret_cast<char*>(&enemy_damage), sizeof(enemy_damage));
    ifs.read(reinterpret_cast<char*>(&health), sizeof(health));
}