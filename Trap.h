#pragma once
#include <ostream>
#include <istream>

class Trap {
private:
    int damage;

public:
    explicit Trap(int dmg);
    int getDamage() const;
    void serialize(std::ostream& ofs) const;
    void deserialize(std::istream& ifs);
};