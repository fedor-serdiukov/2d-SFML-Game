#pragma once

class EnemyBuilding {
private:
    int spawn_interval;
    int turns_until_spawn;
    int enemy_health;
    int enemy_damage;

public:
    EnemyBuilding(int interval, int e_health, int e_damage);
    void tick();
    bool is_ready_to_spawn() const;
    void reset_timer();
    int get_enemy_health() const;
    int get_enemy_damage() const;
    int get_spawn_interval() const;
};