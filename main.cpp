#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>
#include "Field.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "Player.h"
#include "greeting_func.h"


constexpr unsigned int FPS_LIMIT = 30;
constexpr unsigned int ROWS = 15;
constexpr unsigned int COLS = 15;

sf::Vector2i random_pos(int max_x, int max_y) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_x(0, max_x - 1);
    std::uniform_int_distribution<> dis_y(0, max_y - 1);
    return {dis_x(gen), dis_y(gen)};
}

int main() {
    int player_max_health, melee_damage, ranged_damage;

    if (!inputPlayerStats(player_max_health, melee_damage, ranged_damage)) {
        return EXIT_FAILURE;
    }
    Player* player = new Player(player_max_health, melee_damage, ranged_damage);
    std::cout << "Game started\nTo exit, press ESC\nMove with WASD, switch mode with SPACE\n";

    Field field(ROWS, COLS);

    std::vector<sf::Vector2i> blocked;
    std::vector<sf::Vector2i> slowing;
    std::vector<std::pair<sf::Vector2i, EnemyBuilding*>> buildings;
    std::vector<std::pair<sf::Vector2i, Enemy*>> initial_enemies;

    for (int i = 0; i < 25; ++i) {
        sf::Vector2i pos = random_pos(COLS, ROWS);
        if (pos.x == 0 && pos.y == 0) continue;
        blocked.push_back(pos);
    }
    for (int i = 0; i < 30; ++i) {
        sf::Vector2i pos = random_pos(COLS, COLS);
        if (pos.x == 0 && pos.y == 0) continue;
        slowing.push_back(pos);
    }
    for (int i = 0; i < 1; ++i) {
        sf::Vector2i pos = random_pos(COLS, ROWS);
        if (pos.x == 0 && pos.y == 0) continue;
        EnemyBuilding* b = new EnemyBuilding(10, 30, 10);
        buildings.emplace_back(pos, b);
    }
    for (int i = 0; i < 5; ++i) {
        sf::Vector2i pos = random_pos(COLS, ROWS);
        if (pos.x == 0 && pos.y == 0) continue;
        Enemy* e = new Enemy(30, 10);
        initial_enemies.emplace_back(pos, e);
    }

    field.initialize(player, blocked, slowing, buildings, initial_enemies);
    std::cout << "Player spawned at (0,0), health: " << player->get_health() << std::endl;

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

                if (player_turn && !player->is_slowed()) {
                    sf::Vector2i dir(0, 0);
                    bool action_taken = false;
                    if (keyPressed->code == sf::Keyboard::Key::W) dir = {0, -1};
                    else if (keyPressed->code == sf::Keyboard::Key::S) dir = {0, 1};
                    else if (keyPressed->code == sf::Keyboard::Key::A) dir = {-1, 0};
                    else if (keyPressed->code == sf::Keyboard::Key::D) dir = {1, 0};
                    else if (keyPressed->code == sf::Keyboard::Key::Space) {
                        player->toggle_combat_mode();
                        action_taken = true;
                        std::cout << "Switched to " << (player->get_combat_mode() == CombatMode::Melee ? "Melee" : "Ranged") << " mode" << std::endl;
                    }
                    if (dir != sf::Vector2i(0, 0)) {
                        bool moved = field.move_player(dir);
                        action_taken = moved;
                        if (moved) {
                            sf::Vector2i pos = field.find_player_position();
                            std::cout << "Player moved to (" << pos.x << ", " << pos.y << "), health: " << player->get_health() << std::endl;
                        }
                    }
                    if (action_taken) {
                        player_turn = false;
                    }
                }
            }


            if (event.getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int tileX = mousePos.x / static_cast<int>(tileSize + spacing);
                int tileY = mousePos.y / static_cast<int>(tileSize + spacing);
                if (tileX >= 0 && tileX < COLS && tileY >= 0 && tileY < ROWS) {
                    std::cout << "Clicked tile (" << tileX << ", " << tileY << ")" << std::endl;
                }
            }
        }

        if (!player_turn) {
            field.move_enemies();
            field.process_buildings();
            player->decrement_slow();
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
                switch (cell.getType()) {
                    case CellType::Empty: shape.setTexture(&emptyTex); break;
                    case CellType::Blocked: shape.setTexture(&blockedTex); break;
                    case CellType::Player: shape.setFillColor(sf::Color(0, 0, 255)); break;
                    case CellType::Enemy: shape.setTexture(&enemyTex); break;
                    case CellType::Building: shape.setTexture(&buildingTex); break;
                }

                if (cell.getProperty() == CellProperty::Slowing) {
                    shape.setOutlineColor(sf::Color::Yellow);
                    shape.setOutlineThickness(3.f);
                }
                if (x == hoverX && y == hoverY) {
                    shape.setFillColor(sf::Color(200, 200, 255));
                }
                window.draw(shape);
            }
        }
        window.display();
    }
    return 0;
}