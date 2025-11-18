#pragma once
#include "ISpell.h"

class DirectDamageSpell : public ISpell {
private:
    int damage;
    int range;
    std::string name;
    std::string description;

public:
    DirectDamageSpell(int dmg, int rng, std::string n, std::string desc);
    std::unique_ptr<ISpell> clone() const override;
    std::string getName() const override;
    std::string getDescription() const override;
    void setRange(int range) override;
    int getRange() const override;
    bool use(Player& player, Field& field, sf::Vector2i targetPos) override;
    void upgrade() override;
    int getDamage() const override { return damage; }
    void setDamage(int dmg) override { damage = dmg; }
    int getSpellID() const override { return 0; }
    void serialize(std::ostream& ofs) const override;
    void deserialize(std::istream& ifs) override;
};