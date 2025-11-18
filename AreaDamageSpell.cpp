#include "AreaDamageSpell.h"
#include "Field.h"
#include "Player.h"
#include <cmath>
#include <iostream>

AreaDamageSpell::AreaDamageSpell(int dmg, int rng, std::string n, std::string desc)
    : damage(dmg), range(rng), name(std::move(n)), description(std::move(desc)) {}

std::string AreaDamageSpell::getName() const { return name; }
std::string AreaDamageSpell::getDescription() const { return description; }
int AreaDamageSpell::getRange() const { return range; }

bool AreaDamageSpell::use(Player& player, Field& field, sf::Vector2i targetPos) {
    sf::Vector2i playerPos = field.find_player_position();
    if (playerPos.x < 0) return false;

    int distance = std::abs(playerPos.x - targetPos.x) + std::abs(playerPos.y - targetPos.y);
    if (distance > range) {
        std::cout << "Out of radius" << std::endl;
        return false;
    }

    std::cout << "Using AoE at (" << targetPos.x << ", " << targetPos.y << ")..." << std::endl;

    for (int y = targetPos.y; y < targetPos.y + 2; ++y) {
        for (int x = targetPos.x; x < targetPos.x + 2; ++x) {
            sf::Vector2i currentPos = {x, y};
            if (!field.is_valid_position(currentPos)) continue;
            Cell& cell = field.get_cell(x, y);
            if (cell.getType() == CellType::Enemy) {
                field.damageEnemyAt(currentPos, damage);
                std::cout << "... enemy is damaged at (" << x << ", " << y << ")" << std::endl;
            } else if (cell.getType() == CellType::Building) {
                field.damageBuildingAt(currentPos, damage);
                std::cout << "... building is damaged at (" << x << ", " << y << ")" << std::endl;
            }
            else if (cell.getType() == CellType::Ally) {
                field.damageAllyAt(currentPos, damage);
                std::cout << "... ally is damaged at (" << x << ", " << y << ")" << std::endl;
            }
        }
    }
    return true;
}

std::unique_ptr<ISpell> AreaDamageSpell::clone() const {
    return std::make_unique<AreaDamageSpell>(*this);
}

void AreaDamageSpell::setRange(int range) {
    this->range = range;
}

void AreaDamageSpell::upgrade() {
    damage += 10;
    range += 1;
}

void AreaDamageSpell::serialize(std::ostream& ofs) const {
    ofs.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
    ofs.write(reinterpret_cast<const char*>(&range), sizeof(range));
}

void AreaDamageSpell::deserialize(std::istream& ifs) {
    ifs.read(reinterpret_cast<char*>(&damage), sizeof(damage));
    ifs.read(reinterpret_cast<char*>(&range), sizeof(range));
}