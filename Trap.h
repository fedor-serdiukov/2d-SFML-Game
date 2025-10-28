#pragma once

class Trap {
private:
    int damage;

public:
    explicit Trap(int dmg);
    int getDamage() const;
};