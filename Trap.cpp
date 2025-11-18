#include "Trap.h"

Trap::Trap(int dmg) : damage(dmg) {}

int Trap::getDamage() const {
    return damage;
}

void Trap::serialize(std::ostream& ofs) const {
    ofs.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
}
void Trap::deserialize(std::istream& ifs) {
    ifs.read(reinterpret_cast<char*>(&damage), sizeof(damage));
}