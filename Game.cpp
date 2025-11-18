#include "Game.h"
#include <iostream>
#include <fstream>
#include <filesystem>

Game::Game()
    : window(sf::VideoMode({800, 800}), "OOP"),
      currentState(GameState::Menu),
      player_turn(true),
      selectedSpell(nullptr)
{
    window.setFramerateLimit(30);

    if (!blockedTex.loadFromFile("textures/block.png")) std::cerr << "Err: block.png\n";
    if (!emptyTex.loadFromFile("textures/plain.png")) std::cerr << "Err: plain.png\n";
    if (!buildingTex.loadFromFile("textures/building.png")) std::cerr << "Err: building.png\n";
    if (!enemyTex.loadFromFile("textures/enemy.png")) std::cerr << "Err: enemy.png\n";

    if (!font.openFromFile("textures/arial.ttf")) {
        std::cerr << "Warning: arial.ttf not found.\n";
    }
    currentLevel = 1;
}

void Game::run() {
    while (window.isOpen()) {
        handleInput();
        update();
        render();
    }
}

void Game::resetGame() {
    currentLevel = 1;

    Player* newPlayer = new Player(100, 25, 15, 3);
    newPlayer->getHand().addSpell(spellFactory.createRandomSpell());
    player = newPlayer;

    field = std::make_unique<Field>(25, 25);
    FieldContent content = field->generate_random_content(20, 30, 3, 5, 5, 25, 25, currentLevel);
    field->initialize(player, content);

    currentState = GameState::Playing;
    player_turn = true;
    std::cout << "New Game Started! Level " << currentLevel << "\n";
}

void Game::initLevel() {

}

void Game::handleInput() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (currentState == GameState::LevelUpMenu) {
                bool choiceMade = false;

                if (keyPressed->code == sf::Keyboard::Key::Num1) {
                    player->upgradeMaxHealth(20);
                    choiceMade = true;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num2) {
                    player->upgradeDamage(5);
                    choiceMade = true;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num3) {
                    if (player->getHand().getSpellCount() > 0) {
                        player->upgradeRandomSpellInHand();
                        choiceMade = true;
                    } else {
                        std::cout << "Cannot upgrade spell: hand is empty!\n";
                    }
                }

                if (choiceMade) {
                    nextLevel();
                }
            }
            if (keyPressed->code == sf::Keyboard::Key::Escape) {
                if (currentState == GameState::Playing) {
                    if (selectedSpell) {
                        selectedSpell = nullptr;
                        std::cout << "Spell selection cancelled.\n";
                    } else {
                        currentState = GameState::Menu;
                    }
                } else if (currentState == GameState::Menu) {
                   window.close();
                }
            }

            if (currentState == GameState::Menu || currentState == GameState::GameOver || currentState == GameState::Victory) {
                if (keyPressed->code == sf::Keyboard::Key::N) {
                    resetGame();
                }
                if (keyPressed->code == sf::Keyboard::Key::L) {
                    loadGame("savegame.dat");
                }
                if (currentState == GameState::Menu && keyPressed->code == sf::Keyboard::Key::C) {
                     if (field && player) currentState = GameState::Playing;
                }
            }
            else if (currentState == GameState::Playing) {
                if (keyPressed->code == sf::Keyboard::Key::K) {
                    saveGame("savegame.dat");
                }
                if (player_turn) {
                    processPlayerTurn(*event);
                }
            }
        }

        if (currentState == GameState::Playing) {
            processMouseInput(*event);
        }
    }
}

void Game::processPlayerTurn(const sf::Event& event) {
    const auto* keyEvent = event.getIf<sf::Event::KeyPressed>();
    if (!keyEvent) return;

    if (!player) return;

    if (player->is_slowed()) {
         player->decrement_slow();
         player_turn = false;
         return;
    }

    sf::Vector2i dir(0, 0);
    bool action_taken = false;

    if (keyEvent->code >= sf::Keyboard::Key::Num1 && keyEvent->code <= sf::Keyboard::Key::Num3) {
        int idx = static_cast<int>(keyEvent->code) - static_cast<int>(sf::Keyboard::Key::Num1);
        selectedSpell = player->getHand().getSpell(idx);
        if (selectedSpell) std::cout << "Selected: " << selectedSpell->getName() << "\n";
    }
    else if (keyEvent->code == sf::Keyboard::Key::B) {
        if (player->get_score() >= 50) {
            if(player->getHand().addSpell(spellFactory.createRandomSpell())) {
                player->change_score(-50);
                action_taken = true;
            } else std::cout << "Hand full!\n";
        } else std::cout << "Need 50 pts\n";
    }
    else if (keyEvent->code == sf::Keyboard::Key::Space) {
        player->toggle_combat_mode();
        action_taken = true;
    }
    else {
        if (keyEvent->code == sf::Keyboard::Key::W) dir = {0, -1};
        else if (keyEvent->code == sf::Keyboard::Key::S) dir = {0, 1};
        else if (keyEvent->code == sf::Keyboard::Key::A) dir = {-1, 0};
        else if (keyEvent->code == sf::Keyboard::Key::D) dir = {1, 0};

        if (dir != sf::Vector2i(0,0)) {
            if (field->move_player(dir)) {
                action_taken = true;
            }
        }
    }

    if (action_taken) {
        player_turn = false;
        selectedSpell = nullptr;
    }
}

void Game::processMouseInput(const sf::Event& event) {
    if (!player_turn || !selectedSpell || !player) return;

    const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>();
    if (mouseBtn && mouseBtn->button == sf::Mouse::Button::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        int tx = mousePos.x / static_cast<int>(tileSize + spacing);
        int ty = mousePos.y / static_cast<int>(tileSize + spacing);

        if (field->is_valid_position({tx, ty})) {
            if (!selectedSpell->isBuffSpell()) {
                player->applyBuffToSpell(selectedSpell);
            }
            if (selectedSpell->use(*player, *field, {tx, ty})) {
                 if (!selectedSpell->isBuffSpell()) player->resetBuffCharges();
                 player_turn = false;
                 selectedSpell = nullptr;
            }
        }
    }
}

void Game::update() {
    if (currentState != GameState::Playing || !field || !player) return;

    if (!player_turn) {
        std::cout << "--- Enemy Turn ---\n";
        field->move_allies();
        field->move_enemies();
        field->process_buildings();
        field->process_towers();
        player->decrement_slow();

        if (player->getKillCount() >= 3) {
            player->resetKillCount();
            if(player->getHand().addSpell(spellFactory.createRandomSpell()))
                std::cout << "Bonus Spell for Kills!\n";
        }

        player_turn = true;
        std::cout << "--- Player Turn ---\n";
    }

    if (field->is_game_over()) {
        currentState = GameState::GameOver;
        std::cout << "GAME OVER\n";
    } else if (player->get_score() > 30) {
        currentState = GameState::LevelUpMenu;
        std::cout << "Level Complete! Choose an upgrade.\n";
        player->change_score(-30);
    }
}

void Game::render() {
    window.clear(sf::Color::Black);

    if (currentState == GameState::Menu) {
        drawMenu();
    }
    else if (currentState == GameState::LevelUpMenu) {
        drawLevelUpMenu();
    }
    else if (currentState == GameState::Playing && field) {
        for(int y=0; y<field->get_rows(); ++y) {
            for(int x=0; x<field->get_cols(); ++x) {
                float px = x * (tileSize + spacing);
                float py = y * (tileSize + spacing);
                sf::RectangleShape shape({tileSize, tileSize});
                shape.setPosition({px, py});

                Cell& cell = field->get_cell(x, y);

                switch(cell.getType()) {
                    case CellType::Empty:
                        if(cell.getTrap()) shape.setFillColor(sf::Color(0, 150, 150));
                        else shape.setTexture(&emptyTex);
                        break;
                    case CellType::Blocked: shape.setTexture(&blockedTex); break;
                    case CellType::Player: shape.setFillColor(sf::Color::Blue); break;
                    case CellType::Enemy: shape.setTexture(&enemyTex); break;
                    case CellType::Building: shape.setTexture(&buildingTex); break;
                    case CellType::Tower: shape.setFillColor(sf::Color::Red); break;
                    case CellType::Ally: shape.setFillColor(sf::Color::Green); break;
                }

                if(cell.getProperty() == CellProperty::Slowing) {
                    shape.setOutlineColor(sf::Color::Yellow);
                    shape.setOutlineThickness(2.f);
                }
                window.draw(shape);
            }
        }
        drawUI();
    }
    else if (currentState == GameState::GameOver) {
        sf::Text text(font, "GAME OVER\n[N] New Game\n[L] Load", 30);
        text.setFillColor(sf::Color::Red);
        text.setPosition({200, 300});
        window.draw(text);
    }
    else if (currentState == GameState::Victory) {
        sf::Text text(font, "VICTORY!\n[N] New Game", 30);
        text.setFillColor(sf::Color::Yellow);
        text.setPosition({250, 300});
        window.draw(text);
    }

    window.display();
}

void Game::drawMenu() {
    sf::Text text(font, "MAIN MENU\n[N] New Game\n[L] Load Game\n[ESC] Exit\n[C] Continue", 30);
    text.setPosition({200, 200});
    window.draw(text);
}

void Game::drawUI() {
    if (!player) return;
    sf::Text stats(font, "", 20);
    stats.setPosition({10, 10});
    stats.setString("Lvl: " + std::to_string(currentLevel) + "|" +
                    "HP: " + std::to_string(player->get_health()) +
                    " | Score: " + std::to_string(player->get_score()) +
                    " | Mode: " + (player->get_combat_mode() == CombatMode::Melee ? "Melee" : "Ranged"));
    stats.setFillColor(sf::Color::White);
    stats.setOutlineColor(sf::Color::Black);
    stats.setOutlineThickness(1);
    window.draw(stats);

    sf::Text help(font, "[K] Save | [B] Shop | [ESC] Menu", 15);
    help.setPosition({10, static_cast<float>(window.getSize().y) - 30});
    window.draw(help);
}

void Game::saveGame(const std::string& filename) {
    try {
        std::ofstream ofs(filename, std::ios::binary);
        if (!ofs) throw std::runtime_error("Cannot open file for writing");

        int magic = 0xDEADBEEF;
        ofs.write(reinterpret_cast<const char*>(&magic), sizeof(magic));
        ofs.write(reinterpret_cast<const char*>(&player_turn), sizeof(player_turn));

        if(field) field->serialize(ofs);

        std::cout << "Game Saved!\n";
    } catch (const std::exception& e) {
        std::cerr << "Save Error: " << e.what() << std::endl;
    }
}

void Game::loadGame(const std::string& filename) {
    try {
        if (!std::filesystem::exists(filename)) {
            throw std::runtime_error("Save file not found");
        }

        std::ifstream ifs(filename, std::ios::binary);
        if (!ifs) throw std::runtime_error("Cannot open file");

        int magic;
        ifs.read(reinterpret_cast<char*>(&magic), sizeof(magic));
        if (magic != 0xDEADBEEF) throw std::runtime_error("Corrupted save file");

        ifs.read(reinterpret_cast<char*>(&player_turn), sizeof(player_turn));

        field = std::make_unique<Field>(25, 25);

        field->deserialize(ifs, spellFactory);

        player = field->getPlayer();

        if (!player) throw std::runtime_error("Player failed to load");

        currentState = GameState::Playing;
        std::cout << "Game Loaded!\n";

    } catch (const std::exception& e) {
        std::cerr << "Load Error: " << e.what() << std::endl;
        currentState = GameState::Menu;
    }
}

void Game::nextLevel() {
    currentLevel++;
    std::cout << "\n=== TRANSITION TO LEVEL " << currentLevel << " ===\n";

    player->prepareForNextLevel();
    int rows = field->get_rows();
    int cols = field->get_cols();

    std::cout << "Field Size remains: " << cols << "x" << rows << "\n";

    Player* playerCopy = new Player(*player);

    field = std::make_unique<Field>(rows, cols);

    player = playerCopy;

    int blocks = 10 + currentLevel * 2;
    int slows = 15 + currentLevel * 2;
    int buildings = 2 + (currentLevel / 2);
    int enemies = 3 + currentLevel;
    int towers = 3 + currentLevel;

    FieldContent content = field->generate_random_content(
        blocks, slows, buildings, enemies, towers, cols, rows, currentLevel
    );

    field->initialize(player, content);

    player_turn = true;
    currentState = GameState::Playing;
}

void Game::drawLevelUpMenu() {
    sf::Text title(font, "LEVEL COMPLETE!", 40);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition({200, 100});

    sf::Text subtitle(font, "Choose an upgrade for the next level:", 25);
    subtitle.setPosition({150, 200});

    std::string opt1 = "[1] Upgrade Max Health (+20)";
    std::string opt2 = "[2] Upgrade Damage (+5)";
    std::string opt3 = "[3] Upgrade Random Spell";

    sf::Text t1(font, opt1, 30); t1.setPosition({150, 300});
    sf::Text t2(font, opt2, 30); t2.setPosition({150, 350});
    sf::Text t3(font, opt3, 30); t3.setPosition({150, 400});

    if (player->getHand().getSpellCount() == 0) {
        t3.setFillColor(sf::Color(100, 100, 100));
    }

    window.draw(title);
    window.draw(subtitle);
    window.draw(t1);
    window.draw(t2);
    window.draw(t3);
}