#include "Hand.h"
#include "SpellFactory.h"

Hand::Hand(size_t maxSize) : maxSize(maxSize) {}

Hand::Hand(const Hand& other) : maxSize(other.maxSize) {
    for (const auto& spell : other.spells) {
        if (spell) {
            this->spells.push_back(spell->clone());
        } else {
            this->spells.push_back(nullptr);
        }
    }
}

bool Hand::addSpell(std::unique_ptr<ISpell> spell) {
    if (spells.size() < maxSize) {
        spells.push_back(std::move(spell));
        return true;
    }
    return false;
}

ISpell* Hand::getSpell(size_t index) const {
    if (index < spells.size()) {
        return spells[index].get();
    }
    return nullptr;
}

size_t Hand::getSpellCount() const { return spells.size(); }
size_t Hand::getMaxSize() const { return maxSize; }

void Hand::serialize(std::ostream& ofs) const {
    int spellCount = static_cast<int>(spells.size());
    ofs.write(reinterpret_cast<const char*>(&spellCount), sizeof(spellCount));

    for (const auto& spell : spells) {
        int id = spell->getSpellID();
        ofs.write(reinterpret_cast<const char*>(&id), sizeof(id));
        spell->serialize(ofs);
    }
}

void Hand::deserialize(std::istream& ifs, SpellFactory& factory) {
    spells.clear();
    int spellCount;
    ifs.read(reinterpret_cast<char*>(&spellCount), sizeof(spellCount));

    for (int i = 0; i < spellCount; ++i) {
        int id;
        ifs.read(reinterpret_cast<char*>(&id), sizeof(id));

        std::unique_ptr<ISpell> spell = factory.createSpellByID(id);
        if (spell) {
            spell->deserialize(ifs);
            spells.push_back(std::move(spell));
        }
    }
}

void Hand::removeRandomSpells(int count) {
    if (count <= 0) return;
    if (static_cast<size_t>(count) >= spells.size()) {
        spells.clear();
        return;
    }

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(spells.begin(), spells.end(), g);

    for (int i = 0; i < count; ++i) {
        spells.pop_back();
    }
}