#pragma once
#include "ISpell.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "BuffSpell.h"
#include <memory>
#include <random>

class SpellFactory {
private:
    std::mt19937 rng;

public:
    SpellFactory() : rng(std::random_device{}()) {}

    std::unique_ptr<ISpell> createRandomSpell() {
        std::uniform_int_distribution<> dist(0, 4);
        int type = dist(rng);
        return createSpellByID(type);
    }

    std::unique_ptr<ISpell> createSpellByID(int id) {
        switch (id) {
            case 0: return std::make_unique<DirectDamageSpell>(0, 0, "Fireball", "Desc");
            case 1: return std::make_unique<AreaDamageSpell>(0, 0, "Meteor", "Desc");
            case 2: return std::make_unique<TrapSpell>(0, 0, "Trap", "Desc");
            case 3: return std::make_unique<SummonSpell>(0, 0, 0, "Golem", "Desc");
            case 4: return std::make_unique<BuffSpell>("Empower", "Desc");
            default: return nullptr;
        }
    }
};