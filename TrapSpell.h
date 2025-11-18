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
    bool use(Player& player, Field& field, sf::Vector2i targetPos) override;
    int getDamage() const override;
    void setDamage(int damage) override;
    void upgrade() override;

    // Новые методы
    int getSpellID() const override { return 2; }
    void serialize(std::ostream& ofs) const override;
    void deserialize(std::istream& ifs) override;
};