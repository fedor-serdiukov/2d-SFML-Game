#pragma once

class Ally {
private:
    int health;
    int damage;

public:
    Ally(int hp, int dmg);
    Ally(const Ally& other);
    int getHealth() const;
    int getDamage() const;
    void changeHealth(int amount);
};