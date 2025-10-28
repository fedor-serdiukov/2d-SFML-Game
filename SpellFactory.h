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

        if (type == 0) {
            return std::make_unique<DirectDamageSpell>(20, 5, "Fireball", "Hurls a ball of fire (20 dmg).");
        } else if (type == 1) {
            return std::make_unique<AreaDamageSpell>(10, 4, "Meteor", "Calls a 2x2 meteor (10 dmg).");
        } else if (type == 2) {
            return std::make_unique<TrapSpell>(40, 3, "Trap", "Lays a 40 dmg trap.");
        } else if (type == 3) {
            return std::make_unique<SummonSpell>(20, 5, 1, "Golem", "Summons an ally (20hp, 5dmg).");
        } else {
            return std::make_unique<BuffSpell>("Empower", "Empowers your next spell.");
        }
    }
};