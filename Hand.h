#pragma once
#include "ISpell.h"
#include <vector>
#include <memory>
#include "SpellFactory.h"
#include <ostream>
#include <istream>

class Hand {
private:
    std::vector<std::unique_ptr<ISpell>> spells;
    size_t maxSize;

public:
    explicit Hand(size_t maxSize);
    Hand(const Hand& other);
    bool addSpell(std::unique_ptr<ISpell> spell);
    ISpell* getSpell(size_t index) const;
    size_t getSpellCount() const;
    size_t getMaxSize() const;
    void serialize(std::ostream& ofs) const;
    void deserialize(std::istream& ifs, SpellFactory& factory);
    void removeRandomSpells(int count);
};