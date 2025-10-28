#include "BuffSpell.h"
#include "Player.h"
#include <iostream>

BuffSpell::BuffSpell(std::string n, std::string desc)
    : name(std::move(n)), description(std::move(desc)) {}

std::unique_ptr<ISpell> BuffSpell::clone() const {
    return std::make_unique<BuffSpell>(*this);
}

std::string BuffSpell::getName() const { return name; }
std::string BuffSpell::getDescription() const { return description; }
int BuffSpell::getRange() const { return 0; }

bool BuffSpell::use(Player& player, Field& field, sf::Vector2i targetPos) {
    player.addBuffCharge();
    std::cout << "Заклинание улучшение применено. Зарядов: " << player.getBuffCharges() << std::endl;
    return true;
}
