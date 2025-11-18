#pragma once
#include <ostream>
#include <istream>

class Enemy {
private:
    int health;
    int damage;
public:
    Enemy(int health, int damage);
    int get_health() const;
    int get_damage() const;
    void change_health(int delta);
    void serialize(std::ostream& ofs) const;
    void deserialize(std::istream& ifs);
};