#include "SummonSpell.h"
#include "Field.h"
#include "Player.h"
#include "Ally.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

SummonSpell::SummonSpell(int hp, int dmg, int count, std::string n, std::string desc)
    : allyHealth(hp), allyDamage(dmg), summonsCount(count),
      name(std::move(n)), description(std::move(desc)) {}

std::unique_ptr<ISpell> SummonSpell::clone() const {
    return std::make_unique<SummonSpell>(*this);
}

std::string SummonSpell::getName() const { return name; }
std::string SummonSpell::getDescription() const { return description; }
int SummonSpell::getRange() const { return 0; }

bool SummonSpell::use(Player& player, Field& field, sf::Vector2i targetPos) {
    sf::Vector2i playerPos = field.find_player_position();
    if (playerPos.x < 0) return false;

    std::vector<sf::Vector2i> directions = {{0,-1}, {0,1}, {-1,0}, {1,0}};
    std::vector<sf::Vector2i> emptyCells;

    for (auto dir : directions) {
        sf::Vector2i pos = playerPos + dir;
        if (field.is_valid_position(pos) && field.get_cell(pos.x, pos.y).getType() == CellType::Empty) {
            emptyCells.push_back(pos);
        }
    }

    if (emptyCells.empty()) {
        std::cout << "No space to summon allies!" << std::endl;
        return false;
    }

    std::shuffle(emptyCells.begin(), emptyCells.end(), Field::get_rng());

    int spawnedCount = 0;
    for (const auto& pos : emptyCells) {
        if (spawnedCount >= summonsCount) break;
        Ally* newAlly = new Ally(allyHealth, allyDamage);
        field.addAlly(newAlly, pos);
        spawnedCount++;
    }

    std::cout << "Summoned " << spawnedCount << " allies." << std::endl;
    return true;
}

void SummonSpell::setSummonsCount(int count) {
    summonsCount = count;
}

void SummonSpell::upgrade() {
    allyDamage += 10;
    allyHealth += 10;
}

void SummonSpell::serialize(std::ostream& ofs) const {
    ofs.write(reinterpret_cast<const char*>(&allyDamage), sizeof(allyDamage));
    ofs.write(reinterpret_cast<const char*>(&allyHealth), sizeof(allyHealth));
    ofs.write(reinterpret_cast<const char*>(&summonsCount), sizeof(summonsCount));
}

void SummonSpell::deserialize(std::istream& ifs) {
    ifs.read(reinterpret_cast<char*>(&allyDamage), sizeof(allyDamage));
    ifs.read(reinterpret_cast<char*>(&allyHealth), sizeof(allyHealth));
    ifs.read(reinterpret_cast<char*>(&summonsCount), sizeof(summonsCount));
}