#include "DirectDamageSpell.h"
#include "Field.h" // Нужен полный заголовок для Field::damageEnemyAt
#include "Player.h"
#include <cmath> // для std::abs
#include <iostream>

DirectDamageSpell::DirectDamageSpell(int dmg, int rng, std::string n, std::string desc)
    : damage(dmg), range(rng), name(std::move(n)), description(std::move(desc)) {}

std::string DirectDamageSpell::getName() const { return name; }
std::string DirectDamageSpell::getDescription() const { return description; }
int DirectDamageSpell::getRange() const { return range; }

bool DirectDamageSpell::use(Player& player, Field& field, sf::Vector2i targetPos) {
    sf::Vector2i playerPos = field.find_player_position();
    if (playerPos.x < 0) return false; // Игрок не найден

    // Проверка радиуса (Манхэттенское расстояние)
    int distance = std::abs(playerPos.x - targetPos.x) + std::abs(playerPos.y - targetPos.y);
    if (distance > range) {
        std::cout << "Цель вне радиуса действия заклинания." << std::endl;
        return false;
    }

    if (!field.is_valid_position(targetPos)) return false; // Клик за пределами поля

    // Проверка цели
    Cell& targetCell = field.get_cell(targetPos.x, targetPos.y);
    CellType type = targetCell.getType();

    if (type == CellType::Enemy) {
        field.damageEnemyAt(targetPos, damage);
        std::cout << "Нанесено " << damage << " урона врагу в (" << targetPos.x << ", " << targetPos.y << ")" << std::endl;
        return true; // Заклинание успешно применено
    } else if (type == CellType::Building) {
        field.damageBuildingAt(targetPos, damage);
        std::cout << "Нанесено " << damage << " урона зданию в (" << targetPos.x << ", " << targetPos.y << ")" << std::endl;
        return true; // Заклинание успешно применено
    }

    // Неверная цель
    std::cout << "Неверная цель. Нужно целиться во врага или здание." << std::endl;
    return false;
}

std::unique_ptr<ISpell> DirectDamageSpell::clone() const {
    // Используем стандартный конструктор копирования, созданный компилятором
    return std::make_unique<DirectDamageSpell>(*this);
}