#pragma once
#include "ISpell.h"

class AreaDamageSpell : public ISpell {
private:
    int damage;
    int range;
    std::string name;
    std::string description;

public:
    AreaDamageSpell(int dmg, int rng, std::string n, std::string desc);
    std::unique_ptr<ISpell> clone() const override;
    std::string getName() const override;
    std::string getDescription() const override;
    int getRange() const override;
    bool use(Player& player, Field& field, sf::Vector2i targetPos) override;
    void setRange(int range) override;
    void upgrade() override;
    int getSpellID() const override { return 1; }
    void serialize(std::ostream& ofs) const override;
    void deserialize(std::istream& ifs) override;
};