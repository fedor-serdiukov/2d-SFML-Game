#pragma once
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include <SFML/Graphics.hpp>

class Trap;
class EnemyTower;
class Ally;

enum class CellType {
    Empty,
    Player,
    Enemy,
    Building,
    Blocked,
    Tower,
    Ally
};

enum class CellProperty {
    None,
    Slowing
};

class Cell {
private:
    CellType type = CellType::Empty;
    CellProperty property = CellProperty::None;
    Player* player = nullptr;
    Enemy* enemy = nullptr;
    EnemyBuilding* building = nullptr;
    static sf::Texture emptyTexture;
    static sf::Texture blockedTexture;
    static sf::Texture slowingOverlay;
    static bool texturesLoaded;
    static void loadTextures();
    Trap* trap = nullptr;
    EnemyTower* tower = nullptr;
    Ally* ally = nullptr;
public:
    Cell(CellType t = CellType::Empty, CellProperty p = CellProperty::None);
    void draw(sf::RenderWindow& window, float x, float y, float tileSize);
    void setType(CellType t);
    CellType getType() const;
    void setProperty(CellProperty p);
    CellProperty getProperty() const;
    void setPlayer(Player* p);
    Player* getPlayer() const;
    void setEnemy(Enemy* e);
    Enemy* getEnemy() const;
    void setBuilding(EnemyBuilding* b);
    EnemyBuilding* getBuilding() const;
    void setTrap(Trap* t);
    Trap* getTrap() const;
    void setTower(EnemyTower* t);
    EnemyTower* getTower() const;
    void setAlly(Ally* a);
    Ally* getAlly() const;
    void clear();
};