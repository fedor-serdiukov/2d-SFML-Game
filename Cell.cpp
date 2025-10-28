#include "Cell.h"
#include "Trap.h"
#include "EnemyTower.h"
#include "Ally.h"

Cell::Cell(CellType t, CellProperty p)
    : type(t), property(p),
      player(nullptr), enemy(nullptr), building(nullptr),
      trap(nullptr), tower(nullptr), ally(nullptr)
{}

void Cell::setType(CellType t) { type = t; }
CellType Cell::getType() const { return type; }
void Cell::setProperty(CellProperty p) { property = p; }
CellProperty Cell::getProperty() const { return property; }
void Cell::setPlayer(Player* p) { player = p; }
Player* Cell::getPlayer() const { return player; }
void Cell::setEnemy(Enemy* e) { enemy = e; }
Enemy* Cell::getEnemy() const { return enemy; }
void Cell::setBuilding(EnemyBuilding* b) { building = b; }
EnemyBuilding* Cell::getBuilding() const { return building; }

void Cell::setTrap(Trap* t) { trap = t; }
Trap* Cell::getTrap() const { return trap; }
void Cell::setTower(EnemyTower* t) { tower = t; }
EnemyTower* Cell::getTower() const { return tower; }

void Cell::setAlly(Ally* a) { ally = a; }
Ally* Cell::getAlly() const { return ally; }
void Cell::clear() {
    type = CellType::Empty;
    property = CellProperty::None;
    player = nullptr;
    enemy = nullptr;
    building = nullptr;
    trap = nullptr;
    tower = nullptr;
    ally = nullptr;
}