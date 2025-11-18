#pragma once
#include <ostream>
#include <istream>

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
    void serialize(std::ostream& ofs) const;
    void deserialize(std::istream& ifs);
};