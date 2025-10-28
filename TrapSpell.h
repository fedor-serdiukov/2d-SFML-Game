#pragma once
#include "ISpell.h"

class TrapSpell : public ISpell {
private:
    int damage;
    int range;
    std::string name;
    std::string description;

public:
    TrapSpell(int dmg, int rng, std::string n, std::string desc);

    std::unique_ptr<ISpell> clone() const override;
    std::string getName() const override;
    std::string getDescription() const override;
    int getRange() const override;

    /**
     * @brief Размещает ловушку на пустой клетке.
     */
    bool use(Player& player, Field& field, sf::Vector2i targetPos) override;
};