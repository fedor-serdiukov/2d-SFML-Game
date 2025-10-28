#pragma once
#include "ISpell.h"
#include <vector>
#include <memory> // для std::unique_ptr

class Hand {
private:
    std::vector<std::unique_ptr<ISpell>> spells;
    size_t maxSize;

public:
    explicit Hand(size_t maxSize);
    Hand(const Hand& other);
    /**
     * @brief Добавляет заклинание в руку.
     * @return true, если заклинание добавлено, false, если рука полна.
     */
    bool addSpell(std::unique_ptr<ISpell> spell);

    /**
     * @brief Получает (не-владеющий) указатель на заклинание по индексу.
     * @return Указатель на ISpell или nullptr, если индекс неверен.
     */
    ISpell* getSpell(size_t index) const;

    size_t getSpellCount() const;
    size_t getMaxSize() const;
};