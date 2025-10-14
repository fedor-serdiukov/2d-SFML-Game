#pragma once

enum class CombatMode {
    Melee,
    Ranged
};

class Player {
private:
    int health = 0;
    int max_health = 0;
    int melee_damage = 0;
    int ranged_damage = 0;
    int score = 0;
    CombatMode combat_mode = CombatMode::Melee;
    bool switching_mode = false;
    int slowed_turns = 0;
public:
    Player(int max_health, int melee_damage, int ranged_damage);
    static bool inputPlayerStats(int& player_max_health, int& melee_damage, int& ranged_damage);
    int get_health() const;
    int get_max_health() const;
    int get_damage() const;
    int get_score() const;
    CombatMode get_combat_mode() const;
    void toggle_combat_mode();
    bool is_switching_mode() const;
    void reset_switching_mode();
    void set_max_health(int max_health);
    void change_health(int delta);
    void change_score(int add_score);
    bool is_slowed() const;
    void apply_slow(int turns = 1);
    void decrement_slow();
};