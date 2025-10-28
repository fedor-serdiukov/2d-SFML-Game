#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Field.h"
#include "Player.h"
#include "SpellFactory.h"


constexpr unsigned int FPS_LIMIT = 30;
constexpr unsigned int ROWS = 25;
constexpr unsigned int COLS = 25;

enum class GameMode {
    Movement,
    Targeting
};

int main() {
    int player_max_health, melee_damage, ranged_damage;

    if (!Player::inputPlayerStats(player_max_health, melee_damage, ranged_damage)) {
        return EXIT_FAILURE;
    }

    Player* player = new Player(player_max_health, melee_damage, ranged_damage, 5);
    SpellFactory spellFactory;
    player->getHand().addSpell(spellFactory.createRandomSpell());

    Field field(ROWS, COLS);
    FieldContent content = field.generate_random_content(25, 30, 1, 5, 5, COLS, ROWS);
    field.initialize(player, content);


    const float tileSize = 25.f;
    const float spacing = 2.f;
    const float windowWidth = COLS * (tileSize + spacing);
    const float windowHeight = ROWS * (tileSize + spacing);

    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u(windowWidth, windowHeight)),
        "Game",
        sf::Style::Titlebar | sf::Style::Close
    ); window.setFramerateLimit(FPS_LIMIT);

    bool player_turn = true;
    GameMode currentMode = GameMode::Movement;
    ISpell* selectedSpell = nullptr;
    sf::Texture blockedTex, emptyTex, buildingTex, enemyTex;
    if (!blockedTex.loadFromFile("textures/block.png")) {
        std::cerr << "Failed to load block texture!" << std::endl;
        return EXIT_FAILURE;
    }
    if (!emptyTex.loadFromFile("textures/plain.png")) {
        std::cerr << "Failed to load plain texture!" << std::endl;
        return EXIT_FAILURE;
    }
    if (!buildingTex.loadFromFile("textures/building.png")) {
        std::cerr << "Failed to load building texture!" << std::endl;
        return EXIT_FAILURE;
    }
    if (!enemyTex.loadFromFile("textures/enemy.png")) {
        std::cerr << "Failed to load enemy texture!" << std::endl;
        return EXIT_FAILURE;
    }

    while (window.isOpen()) {
        while (std::optional<sf::Event> optEvent = window.pollEvent()) {
            if (!optEvent) continue;
            const auto& event = *optEvent;

            if (event.is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    window.close();
                    return 0;
                }
                if (player_turn && player->is_slowed()) {
                    player->decrement_slow();
                    player_turn = false;
                    currentMode = GameMode::Movement;
                    selectedSpell = nullptr;
                    std::cout << "Player is slowed! Turn skipped.\n";
                    continue;
                }
                if (player_turn) {
                    sf::Vector2i dir(0, 0);
                    bool action_taken = false;
                    if (keyPressed->code >= sf::Keyboard::Key::Num1 && keyPressed->code <= sf::Keyboard::Key::Num3) {
                        int spellIndex = (int)(keyPressed->code) - (int)(sf::Keyboard::Key::Num1);
                        selectedSpell = player->getHand().getSpell(spellIndex);
                        if (selectedSpell) {
                            bool is_buff = selectedSpell->isBuffSpell();
                            if (!is_buff) {
                                player->applyBuffToSpell(*selectedSpell);
                            }
                            currentMode = GameMode::Targeting;
                            std::cout << "Selected spell: " << selectedSpell->getName()
                                      << ". Range: " << selectedSpell->getRange()
                                      << ". Click to cast.\n";
                        } else {
                            std::cout << "No spell in slot " << (spellIndex + 1) << ".\n";
                        }
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::B) {
                        if (player->get_score() >= 50) {
                            bool added = player->getHand().addSpell(spellFactory.createRandomSpell());
                            if (added) {
                                player->change_score(-50);
                                std::cout << "Bought a new spell for 50 points. Turn spent.\n";
                                action_taken = true;
                            } else {
                                std::cout << "Cannot buy spell, your hand is full!\n";
                            }
                        } else {
                            std::cout << "Not enough points! (Need 50)\n";
                        }
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::Space) {
                        player->toggle_combat_mode();
                        action_taken = true;
                        currentMode = GameMode::Movement;
                    }
                    else if (currentMode == GameMode::Movement) {
                        if (keyPressed->code == sf::Keyboard::Key::W) dir = {0, -1};
                        else if (keyPressed->code == sf::Keyboard::Key::S) dir = {0, 1};
                        else if (keyPressed->code == sf::Keyboard::Key::A) dir = {-1, 0};
                        else if (keyPressed->code == sf::Keyboard::Key::D) dir = {1, 0};
                        if (dir != sf::Vector2i(0, 0)) {
                            bool moved = field.move_player(dir);
                            action_taken = moved;
                            if (moved) {
                                sf::Vector2i pos = field.find_player_position();
                                std::cout << "Player moved to (" << pos.x << ", " << pos.y << "), health: " << player->get_health();
                                if (field.get_cell(pos.x, pos.y).getProperty() == CellProperty::Slowing && player->is_slowed()) {
                                    player->apply_slow(2);
                                    std::cout << ", entered Slowing tile! Movement blocked next turn.";
                                }
                                std::cout << std::endl;
                            }
                        }
                    }

                    if (action_taken) {
                        player_turn = false;
                        currentMode = GameMode::Movement;
                        selectedSpell = nullptr;
                    }
                }
            }

            if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    int tileX = mousePos.x / static_cast<int>(tileSize + spacing);
                    int tileY = mousePos.y / static_cast<int>(tileSize + spacing);
                    sf::Vector2i targetPos = {tileX, tileY};
                    if (player_turn && currentMode == GameMode::Targeting && selectedSpell && field.is_valid_position(targetPos))
                    {
                        bool success = selectedSpell->use(*player, field, targetPos);
                        if (success) {
                            player_turn = false;
                        }
                        currentMode = GameMode::Movement;
                        selectedSpell = nullptr;
                    }
                    else if (field.is_valid_position(targetPos)) {
                        std::cout << "Clicked tile (" << tileX << ", " << tileY << ")" << std::endl;
                    }
                }
            }
        }

        if (!player_turn) {
            field.move_enemies();
            field.process_buildings();
            field.process_towers();
            field.move_allies();
            player->decrement_slow();

            if (player->getKillCount() >= 3) {
                player->resetKillCount();
                bool added = player->getHand().addSpell(spellFactory.createRandomSpell());
                if (added) {
                    std::cout << "You earned a new spell for 3 kills!\n";
                } else {
                    std::cout << "You earned a spell, but your hand is full!\n";
                }
            }

            player_turn = true;
            if (field.is_game_over()) {
                std::cout << "Game Over! Final score: " << player->get_score() << std::endl;
                window.close();
            }
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        int hoverX = mousePos.x / static_cast<int>(tileSize + spacing);
        int hoverY = mousePos.y / static_cast<int>(tileSize + spacing);


        for (int y = 0; y < ROWS; ++y) {
            for (int x = 0; x < COLS; ++x) {
                float posX = x * (tileSize + spacing);
                float posY = y * (tileSize + spacing);

                const auto& cell = field.get_cell(x, y);
                sf::RectangleShape shape(sf::Vector2f(tileSize, tileSize));
                shape.setPosition(sf::Vector2f(posX, posY));


                switch (cell.getType()) {
                    case CellType::Empty:
                        if (cell.getTrap()) {
                            shape.setFillColor(sf::Color(0, 150, 150));
                        } else {
                            shape.setTexture(&emptyTex);
                        }
                    break;
                    case CellType::Blocked: shape.setTexture(&blockedTex); break;
                    case CellType::Player: shape.setFillColor(sf::Color(0, 0, 255)); break;
                    case CellType::Enemy: shape.setTexture(&enemyTex); break;
                    case CellType::Building: shape.setTexture(&buildingTex); break;
                    case CellType::Tower: shape.setFillColor(sf::Color(180, 0, 0)); break;
                    case CellType::Ally: shape.setFillColor(sf::Color(0, 255, 0)); break;
                }

                if (cell.getProperty() == CellProperty::Slowing) {
                    shape.setOutlineColor(sf::Color::Yellow);
                    shape.setOutlineThickness(3.f);
                }

                if (currentMode == GameMode::Targeting && x == hoverX && y == hoverY) {
                    shape.setFillColor(sf::Color(255, 100, 100, 150));
                }
                else if (x == hoverX && y == hoverY) {
                    shape.setFillColor(sf::Color(200, 200, 255, 100));
                }
                window.draw(shape);
            }
        }
        window.display();
    }
    return 0;
}