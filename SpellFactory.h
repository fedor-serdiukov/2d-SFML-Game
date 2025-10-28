#pragma once
#include "ISpell.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include <memory>
#include "BuffSpell.h"
#include "SummonSpell.h"
#include <random>

class SpellFactory {
private:
    std::mt19937 rng; // Генератор случайных чисел

public:
    SpellFactory() : rng(std::random_device{}()) {}

    // Создает случайное заклинание
    std::unique_ptr<ISpell> createRandomSpell() {
        std::uniform_int_distribution<> dist(0, 2); // <-- ИЗМЕНЕНО НА 2
        int type = dist(rng);

        if (type == 0) {
            // Прямой урон: урон 20, дальность 5
            return std::make_unique<DirectDamageSpell>(20, 5, "Fireball", "Hurls a ball of fire (20 dmg).");
        } else if (type == 1) { // <-- ИЗМЕНЕНО НА ELSE IF
            // AoE урон: урон 10, дальность 4
            return std::make_unique<AreaDamageSpell>(10, 4, "Meteor", "Calls a 2x2 meteor (10 dmg).");
        } else if (type == 2) {
            return std::make_unique<TrapSpell>(40, 3, "Trap", "Lays a 40 dmg trap.");
        } else if (type == 3) {
            // --- НОВОЕ ЗАКЛИНАНИЕ ПРИЗЫВА ---
            // Здоровье 15, Урон 5, Призыв 1
            return std::make_unique<SummonSpell>(15, 5, 1, "Golem", "Summons a small ally (15 HP, 5 DMG).");
        } else {
            // --- НОВОЕ ЗАКЛИНАНИЕ УЛУЧШЕНИЯ ---
            return std::make_unique<BuffSpell>("Rune", "Buffs your next non-buff spell.");
        }
    }
};