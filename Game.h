#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "Field.h"
#include "Player.h"
#include "SpellFactory.h"

enum class GameState {
    Menu,
    Playing,
    GameOver,
    Victory,
    LevelUpMenu
};

class Game {
private:
    sf::RenderWindow window;
    std::unique_ptr<Field> field;

    Player* player = nullptr;

    SpellFactory spellFactory;

    GameState currentState;

    sf::Texture blockedTex, emptyTex, buildingTex, enemyTex;
    sf::Font font;

    const float tileSize = 25.f;
    const float spacing = 2.f;


    bool player_turn;
    ISpell* selectedSpell;

    void handleInput();
    void update();
    void render();

    void initLevel();
    void resetGame();

    void drawMenu();
    void drawUI();

    void saveGame(const std::string& filename);
    void loadGame(const std::string& filename);

    void processPlayerTurn(const sf::Event& event);
    void processMouseInput(const sf::Event& event);

    int currentLevel = 1;
    void nextLevel();
    void drawLevelUpMenu();
public:
    Game();
    void run();
};