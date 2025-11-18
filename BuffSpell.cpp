#include "BuffSpell.h"
#include "Player.h"
#include "Field.h"
#include <iostream>

BuffSpell::BuffSpell(std::string n, std::string desc, int initialCharge)
    : name(std::move(n)), description(std::move(desc)), chargeValue(initialCharge){}

std::unique_ptr<ISpell> BuffSpell::clone() const {
    return std::make_unique<BuffSpell>(*this);
}

std::string BuffSpell::getName() const { return name; }
std::string BuffSpell::getDescription() const { return description; }
int BuffSpell::getRange() const { return 0; }

bool BuffSpell::use(Player& player, Field& field, sf::Vector2i targetPos) {
    player.addBuffCharge();
    std::cout << "Spell is enhanced! Current buff charges: " << player.getBuffCharges() << std::endl;
    return true;
}

bool BuffSpell::isBuffSpell() const {
    return true;
}

void BuffSpell::upgrade() {
    chargeValue += 1;
    std::cout << "BuffSpell upgraded: now gives +" << chargeValue << " charge.\n";
}

void BuffSpell::serialize(std::ostream& ofs) const {
    ofs.write(reinterpret_cast<const char*>(&chargeValue), sizeof(chargeValue));
}
void BuffSpell::deserialize(std::istream& ifs) {
    ifs.read(reinterpret_cast<char*>(&chargeValue), sizeof(chargeValue));
}