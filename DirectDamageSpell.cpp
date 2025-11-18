#include "DirectDamageSpell.h"
#include "Field.h"
#include "Player.h"
#include <cmath>
#include <iostream>

DirectDamageSpell::DirectDamageSpell(int dmg, int rng, std::string n, std::string desc)
    : damage(dmg), range(rng), name(std::move(n)), description(std::move(desc)) {}

std::string DirectDamageSpell::getName() const { return name; }
std::string DirectDamageSpell::getDescription() const { return description; }
int DirectDamageSpell::getRange() const { return range; }

bool DirectDamageSpell::use(Player& player, Field& field, sf::Vector2i targetPos) {
    sf::Vector2i playerPos = field.find_player_position();
    if (playerPos.x < 0) return false;

    int distance = std::abs(playerPos.x - targetPos.x) + std::abs(playerPos.y - targetPos.y);
    if (distance > range) {
        std::cout << "Out of radius." << std::endl;
        return false;
    }

    if (!field.is_valid_position(targetPos)) return false;

    Cell& targetCell = field.get_cell(targetPos.x, targetPos.y);
    CellType type = targetCell.getType();

    if (type == CellType::Enemy) {
        field.damageEnemyAt(targetPos, damage);
        std::cout << "Enemy got " << damage << " damage at (" << targetPos.x << ", " << targetPos.y << ")" << std::endl;
        return true;
    } else if (type == CellType::Building) {
        field.damageBuildingAt(targetPos, damage);
        std::cout << "Building got " << damage << " damage at (" << targetPos.x << ", " << targetPos.y << ")" << std::endl;
        return true;
    }
    std::cout << "Invalid target. You need to aim at an enemy or a building." << std::endl;
    return false;
}

std::unique_ptr<ISpell> DirectDamageSpell::clone() const {
    return std::make_unique<DirectDamageSpell>(*this);
}

void DirectDamageSpell::setRange(int range) {
    this->range = range;
}

void DirectDamageSpell::upgrade() {
    damage += 10;
    range += 1;
}

void DirectDamageSpell::serialize(std::ostream& ofs) const {
    ofs.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
    ofs.write(reinterpret_cast<const char*>(&range), sizeof(range));
}

void DirectDamageSpell::deserialize(std::istream& ifs) {
    ifs.read(reinterpret_cast<char*>(&damage), sizeof(damage));
    ifs.read(reinterpret_cast<char*>(&range), sizeof(range));
}