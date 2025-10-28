#pragma once

class EnemyTower {
private:
    int damage;
    int range;
    int cooldown;
    int turns_until_ready;

public:
    EnemyTower(int dmg, int rng, int cd);
    void tick();
    bool isReady() const;
    void resetCooldown();
    int getRange() const;
    int getDamage() const;
};