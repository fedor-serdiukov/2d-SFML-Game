#include "SummonSpell.h"
#include "Field.h"
#include "Player.h"
#include "Ally.h"
#include <iostream>
#include <algorithm>
#include <random>

SummonSpell::SummonSpell(int hp, int dmg, int count, std::string n, std::string desc)
    : allyHealth(hp), allyDamage(dmg), summonsCount(count),
      name(std::move(n)), description(std::move(desc)) {}

std::unique_ptr<ISpell> SummonSpell::clone() const {
    return std::make_unique<SummonSpell>(*this);
}

std::string SummonSpell::getName() const { return name; }
std::string SummonSpell::getDescription() const { return description; }
int SummonSpell::getRange() const { return 0; } // Не требует выбора цели

bool SummonSpell::use(Player& player, Field& field, sf::Vector2i targetPos) {
    sf::Vector2i playerPos = field.find_player_position();
    if (playerPos.x < 0) return false;

    std::vector<sf::Vector2i> possible_positions;
    // Соседние 4 клетки
    std::vector<sf::Vector2i> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    for (auto dir : directions) {
        sf::Vector2i pos = playerPos + dir;
        if (field.is_valid_position(pos) && field.get_cell(pos.x, pos.y).getType() == CellType::Empty) {
            possible_positions.push_back(pos);
        }
    }

    if (possible_positions.empty()) {
        std::cout << "Нет места для призыва союзников." << std::endl;
        return false;
    }

    // Призываем столько союзников, сколько можем, до summonsCount
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(possible_positions.begin(), possible_positions.end(), g);

    int count = 0;
    for (const auto& pos : possible_positions) {
        if (count >= summonsCount) break;

        Ally* newAlly = new Ally(allyHealth, allyDamage);
        field.addAlly(newAlly, pos);
        count++;
    }

    if (count > 0) {
        std::cout << "Призвано " << count << " союзников (HP: " << allyHealth << ", DMG: " << allyDamage << ").\n";
        return true;
    }

    return false;
}