#include "EnemyBuilding.h"

EnemyBuilding::EnemyBuilding(int interval, int e_health, int e_damage)
    : spawn_interval(interval), turns_until_spawn(interval), enemy_health(e_health), enemy_damage(e_damage) {}

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