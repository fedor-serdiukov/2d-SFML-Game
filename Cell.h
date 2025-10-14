#pragma once
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include <SFML/Graphics.hpp>

enum class CellType {
    Empty,
    Player,
    Enemy,
    Building,
    Blocked
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
    void clear();
};