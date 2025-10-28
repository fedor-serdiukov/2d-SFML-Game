#pragma once
#include "ISpell.h"

class BuffSpell : public ISpell {
private:
    std::string name;
    std::string description;

public:
    BuffSpell(std::string n, std::string desc);
    std::unique_ptr<ISpell> clone() const override;
    std::string getName() const override;
    std::string getDescription() const override;
    int getRange() const override;
    bool use(Player& player, Field& field, sf::Vector2i targetPos) override;
    bool isBuffSpell() const override;
};