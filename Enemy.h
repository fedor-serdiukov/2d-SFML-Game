#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
private:
    int health;
    int damage;
public:
    Enemy(int health, int damage);
    int get_health() const;
    int get_damage() const;
    void change_health(int delta);
};