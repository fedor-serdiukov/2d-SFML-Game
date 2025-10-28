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

    // Проверка радиуса
    int distance = std::abs(playerPos.x - targetPos.x) + std::abs(playerPos.y - targetPos.y);
    if (distance > range) {
        std::cout << "Цель вне радиуса действия заклинания." << std::endl;
        return false;
    }

    // Согласно требованию #4, заклинание используется всегда, если в радиусе.
    std::cout << "Применение AoE заклинания в (" << targetPos.x << ", " << targetPos.y << ")..." << std::endl;

    // Применяем урон к области 2x2
    for (int y = targetPos.y; y < targetPos.y + 2; ++y) {
        for (int x = targetPos.x; x < targetPos.x + 2; ++x) {
            sf::Vector2i currentPos = {x, y};
            // Проверяем, что клетка в пределах поля
            if (!field.is_valid_position(currentPos)) continue;

            Cell& cell = field.get_cell(x, y);
            if (cell.getType() == CellType::Enemy) {
                field.damageEnemyAt(currentPos, damage);
                std::cout << "... поражен враг в (" << x << ", " << y << ")" << std::endl;
            } else if (cell.getType() == CellType::Building) {
                field.damageBuildingAt(currentPos, damage);
                std::cout << "... поражено здание в (" << x << ", " << y << ")" << std::endl;
            }
        }
    }

    return true; // Ход потрачен
}

std::unique_ptr<ISpell> AreaDamageSpell::clone() const {
    // Используем стандартный конструктор копирования
    return std::make_unique<AreaDamageSpell>(*this);
}