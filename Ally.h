#pragma once
#include <SFML/System/Vector2.hpp>

class Ally {
private:
    int health;
    int damage;

public:
    Ally(int hp, int dmg);

    // Союзник может быть скопирован (для глубокого копирования Field)
    Ally(const Ally& other);

    int getHealth() const;
    int getDamage() const;
    void changeHealth(int amount);
};