#include "Hand.h"

Hand::Hand(size_t maxSize) : maxSize(maxSize) {}

Hand::Hand(const Hand& other) : maxSize(other.maxSize) {
    // Вручную клонируем каждое заклинание из "другой" руки
    for (const auto& spell : other.spells) {
        if (spell) {
            this->spells.push_back(spell->clone());
        } else {
            this->spells.push_back(nullptr); // или просто пропустить
        }
    }
}

bool Hand::addSpell(std::unique_ptr<ISpell> spell) {
    if (spells.size() < maxSize) {
        spells.push_back(std::move(spell));
        return true;
    }
    return false; // Рука полна
}

ISpell* Hand::getSpell(size_t index) const {
    if (index < spells.size()) {
        return spells[index].get();
    }
    return nullptr;
}

size_t Hand::getSpellCount() const { return spells.size(); }
size_t Hand::getMaxSize() const { return maxSize; }