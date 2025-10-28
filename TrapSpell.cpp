#include "TrapSpell.h"
#include "Field.h" // Нужен полный заголовок
#include "Trap.h"  // Включаем Trap
#include "Player.h"
#include <cmath>
#include <iostream>

TrapSpell::TrapSpell(int dmg, int rng, std::string n, std::string desc)
    : damage(dmg), range(rng), name(std::move(n)), description(std::move(desc)) {}

std::unique_ptr<ISpell> TrapSpell::clone() const {
    return std::make_unique<TrapSpell>(*this);
}

std::string TrapSpell::getName() const { return name; }
std::string TrapSpell::getDescription() const { return description; }
int TrapSpell::getRange() const { return range; }

bool TrapSpell::use(Player& player, Field& field, sf::Vector2i targetPos) {
    sf::Vector2i playerPos = field.find_player_position();
    if (playerPos.x < 0) return false;

    // Проверка радиуса
    int distance = std::abs(playerPos.x - targetPos.x) + std::abs(playerPos.y - targetPos.y);
    if (distance > range) {
        std::cout << "Цель вне радиуса действия заклинания." << std::endl;
        return false;
    }

    if (!field.is_valid_position(targetPos)) return false;

    // Ловушку можно ставить только на пустую клетку
    Cell& targetCell = field.get_cell(targetPos.x, targetPos.y);
    if (targetCell.getType() == CellType::Empty && targetCell.getTrap() == nullptr) {

        Trap* newTrap = new Trap(damage);
        field.addTrap(newTrap, targetPos); // Просим поле добавить ловушку

        std::cout << "Ловушка (урон " << damage << ") установлена в ("
                  << targetPos.x << ", " << targetPos.y << ")" << std::endl;
        return true; // Заклинание успешно применено
    }

    std::cout << "Неверная цель. Можно ставить ловушки только на пустые клетки." << std::endl;
    return false;
}