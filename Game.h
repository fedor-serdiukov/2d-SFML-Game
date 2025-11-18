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
    Victory
};

class Game {
private:
    sf::RenderWindow window;
    std::unique_ptr<Field> field;

    // ВАЖНО: Обычный указатель. Владеет игроком класс Field.
    Player* player = nullptr;

    SpellFactory spellFactory;

    GameState currentState;

    // Графические ресурсы
    sf::Texture blockedTex, emptyTex, buildingTex, enemyTex;
    sf::Font font;

    // Параметры отображения
    const float tileSize = 25.f;
    const float spacing = 2.f;

    // Логика управления
    bool player_turn;
    ISpell* selectedSpell;

    void handleInput();
    void update();
    void render();

    void initLevel();
    void resetGame();

    // Меню и интерфейс
    void drawMenu();
    void drawUI();

    // Сохранение и загрузка
    void saveGame(const std::string& filename);
    void loadGame(const std::string& filename);

    // Вспомогательные методы
    void processPlayerTurn(const sf::Event& event);
    void processMouseInput(const sf::Event& event);

public:
    Game();
    void run();
};