#pragma once
#include "ISpell.h"

class SummonSpell : public ISpell {
private:
    int allyHealth;
    int allyDamage;
    int summonsCount;
    std::string name;
    std::string description;

public:
    SummonSpell(int hp, int dmg, int count, std::string n, std::string desc);

    std::unique_ptr<ISpell> clone() const override;
    std::string getName() const override;
    std::string getDescription() const override;
    int getRange() const override;
    bool use(Player& player, Field& field, sf::Vector2i targetPos) override;
    void setSummonsCount(int count) override;
    void upgrade() override;

    int getSpellID() const override { return 3; }
    void serialize(std::ostream& ofs) const override;
    void deserialize(std::istream& ifs) override;
};