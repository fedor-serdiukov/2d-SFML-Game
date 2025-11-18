#pragma once
#include <ostream>
#include <istream>

class EnemyBuilding {
private:
    int spawn_interval;
    int turns_until_spawn;
    int enemy_health;
    int enemy_damage;
    int health;
public:
    EnemyBuilding(int interval, int e_health, int e_damage, int b_health);
    void tick();
    bool is_ready_to_spawn() const;
    void reset_timer();
    int get_enemy_health() const;
    int get_enemy_damage() const;
    int get_spawn_interval() const;
    int get_health() const;
    void change_health(int delta);
    void serialize(std::ostream& ofs) const;
    void deserialize(std::istream& ifs);
};