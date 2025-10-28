#include "Field.h"
#include <random>
#include <algorithm>
#include <iostream>

std::mt19937 Field::rng(std::random_device{}());

sf::Vector2i Field::random_pos(int max_x, int max_y) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_x(0, max_x - 1);
    std::uniform_int_distribution<> dis_y(0, max_y - 1);
    return {dis_x(gen), dis_y(gen)};
}


FieldContent Field::generate_random_content(int blocked_count, int slowing_count,
                                            int building_count, int enemy_count,
                                            int tower_count, int COLS, int ROWS)
{
    FieldContent content;

    for (int i = 0; i < blocked_count; ++i) {
        sf::Vector2i pos = random_pos(COLS, ROWS);
        if (pos.x == 0 && pos.y == 0) continue;
        content.blocked.push_back(pos);
    }

    for (int i = 0; i < slowing_count; ++i) {
        sf::Vector2i pos = random_pos(COLS, ROWS);
        if (pos.x == 0 && pos.y == 0) continue;
        content.slowing.push_back(pos);
    }

    for (int i = 0; i < building_count; ++i) {
        sf::Vector2i pos = random_pos(COLS, ROWS);
        if (pos.x == 0 && pos.y == 0) continue;
        EnemyBuilding* b = new EnemyBuilding(10, 30, 10, 10);
        content.buildings.emplace_back(pos, b);
    }

    for (int i = 0; i < enemy_count; ++i) {
        sf::Vector2i pos = random_pos(COLS, ROWS);
        if (pos.x == 0 && pos.y == 0) continue;
        Enemy* e = new Enemy(30, 10);
        content.initial_enemies.emplace_back(pos, e);
    }

    for (int i = 0; i < tower_count; ++i) {
        sf::Vector2i pos = random_pos(COLS, ROWS);
        if (pos.x == 0 && pos.y == 0) continue;
        // Урон 5, дальность 4, перезарядка 3 хода
        EnemyTower* t = new EnemyTower(5, 4, 3);
        content.towers.emplace_back(pos, t);
    }

    return content;
}

Field::Field(int r, int c) : rows(r), cols(c), grid(r, std::vector<Cell>(c)) {
    if (r < 10 || r > 25 || c < 10 || c > 25) {
        throw std::invalid_argument("Field size must be between 10x10 and 25x25");
    }
}

Field::Field(const Field& other) : rows(other.rows), cols(other.cols) {
    deep_copy(other);
}

Field::Field(Field&& other) noexcept
    : rows(other.rows), cols(other.cols), grid(std::move(other.grid)),
      player(other.player), enemies(std::move(other.enemies)),
      buildings(std::move(other.buildings)),
      towers(std::move(other.towers)),
      traps(std::move(other.traps))
{
    other.player = nullptr;
    other.rows = 0;
    other.cols = 0;
}

Field& Field::operator=(const Field& other) {
    if (this != &other) {
        // --- ОБНОВЛЕННАЯ ОЧИСТКА ---
        for (auto e : enemies) delete e;
        for (auto b : buildings) delete b;
        for (auto t : towers) delete t;
        for (auto t : traps) delete t;
        delete player; // 'player' удаляется здесь, а не в deep_copy

        enemies.clear();
        buildings.clear();
        towers.clear();
        traps.clear();
        player = nullptr;

        rows = other.rows;
        cols = other.cols;
        deep_copy(other);
    }
    return *this;
}

Field& Field::operator=(Field&& other) noexcept {
    if (this != &other) {
        // --- ОБНОВЛЕННАЯ ОЧИСТКА ---
        for (auto e : enemies) delete e;
        for (auto b : buildings) delete b;
        for (auto t : towers) delete t;
        for (auto t : traps) delete t;
        delete player;

        enemies.clear();
        buildings.clear();
        towers.clear();
        traps.clear();

        rows = other.rows;
        cols = other.cols;
        grid = std::move(other.grid);
        player = other.player;
        enemies = std::move(other.enemies);
        buildings = std::move(other.buildings);
        towers = std::move(other.towers); // <-- ПЕРЕМЕЩЕНИЕ
        traps = std::move(other.traps);   // <-- ПЕРЕМЕЩЕНИЕ

        other.rows = 0;
        other.cols = 0;
        other.player = nullptr;
    }
    return *this;
}

Field::~Field() {
    for (auto e : enemies) delete e;
    for (auto b : buildings) delete b;
    for (auto t : towers) delete t; // <-- ОЧИСТКА
    for (auto t : traps) delete t; // <-- ОЧИСТКА
    delete player;
}

void Field::deep_copy(const Field& other) {
    grid = other.grid;
    if (other.player) {
        player = new Player(*other.player);
        sf::Vector2i p_pos = other.find_player_position();
        if (p_pos.x >= 0) {
            grid[p_pos.y][p_pos.x].setPlayer(player);
        }
    }
    for (auto e : other.enemies) {
        Enemy* new_e = new Enemy(*e);
        enemies.push_back(new_e);
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                if (other.grid[y][x].getEnemy() == e) {
                    grid[y][x].setEnemy(new_e);
                }
            }
        }
    }
    for (auto b : other.buildings) {
        EnemyBuilding* new_b = new EnemyBuilding(*b);
        buildings.push_back(new_b);
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                if (other.grid[y][x].getBuilding() == b) {
                    grid[y][x].setBuilding(new_b);
                }
            }
        }
    }

    for (auto t : other.towers) {
        EnemyTower* new_t = new EnemyTower(*t); // Используем конструктор копирования
        towers.push_back(new_t);
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                if (other.grid[y][x].getTower() == t) {
                    grid[y][x].setTower(new_t);
                }
            }
        }
    }

    for (auto t : other.traps) {
        Trap* new_t = new Trap(*t); // Используем конструктор копирования
        traps.push_back(new_t);
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                if (other.grid[y][x].getTrap() == t) {
                    grid[y][x].setTrap(new_t);
                }
            }
        }
    }
}

int Field::get_rows() const { return rows; }
int Field::get_cols() const { return cols; }

Cell& Field::get_cell(int x, int y) {
    if (!is_valid_position({x, y})) throw std::out_of_range("Invalid position");
    return grid[y][x];
}

const Cell& Field::get_cell(int x, int y) const {
    if (!is_valid_position({x, y})) throw std::out_of_range("Invalid position");
    return grid[y][x];
}

void Field::initialize(Player* p, const FieldContent& content) {
    player = p;
    for (auto pos : content.blocked) {
        get_cell(pos.x, pos.y).setType(CellType::Blocked);
    }
    for (auto pos : content.slowing) {
        get_cell(pos.x, pos.y).setProperty(CellProperty::Slowing);
    }
    for (auto& pair : content.buildings) {
        auto pos = pair.first;
        auto b = pair.second;
        buildings.push_back(b);
        auto& cell = get_cell(pos.x, pos.y);
        cell.setType(CellType::Building);
        cell.setBuilding(b);
    }
    for (auto& pair : content.initial_enemies) {
        auto pos = pair.first;
        auto e = pair.second;
        enemies.push_back(e);
        auto& cell = get_cell(pos.x, pos.y);
        cell.setType(CellType::Enemy);
        cell.setEnemy(e);
    }
    for (auto& pair : content.towers) {
        auto pos = pair.first;
        auto t = pair.second;
        towers.push_back(t);
        auto& cell = get_cell(pos.x, pos.y);
        cell.setType(CellType::Tower);
        cell.setTower(t);
    }

    auto& start_cell = get_cell(0, 0);
    start_cell.setType(CellType::Player);
    start_cell.setPlayer(player);
    std::cout << "Player spawned at (0,0), health: " << player->get_health() << std::endl;
}

sf::Vector2i Field::find_player_position() const {
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if (grid[y][x].getType() == CellType::Player) {
                return {x, y};
            }
        }
    }
    return {-1, -1};
}

bool Field::move_player(sf::Vector2i direction) {
    sf::Vector2i pos = find_player_position();
    if (pos.x < 0) return false;
    sf::Vector2i new_pos = pos + direction;
    if (!is_valid_position(new_pos)) return false;

    auto& target = get_cell(new_pos.x, new_pos.y);
    if (target.getType() == CellType::Blocked ||
        target.getType() == CellType::Building ||
        target.getType() == CellType::Tower) // <-- Нельзя наступать на башню
    {
        return false;
    }
    if (target.getType() == CellType::Enemy) {
        damageEnemyAt(new_pos, player->get_damage()); // <-- ИЗМЕНЕНО
        return true;
    }

    auto& current = get_cell(pos.x, pos.y);
    current.clear();
    target.setType(CellType::Player);
    target.setPlayer(player);

    if (target.getProperty() == CellProperty::Slowing) {
        player->apply_slow(1);
    }

    return true;
}

void Field::move_enemies() {
    sf::Vector2i player_pos = find_player_position();
    if (player_pos.x < 0) return;

    std::vector<Enemy*> enemies_copy = enemies;

    for (auto e : enemies_copy) {
        sf::Vector2i pos{-1, -1};

        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                if (grid[y][x].getEnemy() == e) {
                    pos = {x, y};
                    break;
                }
            }
            if (pos.x >= 0) break;
        }

        if (pos.x < 0) continue;

        std::vector<sf::Vector2i> directions = {{0,-1}, {0,1}, {-1,0}, {1,0}};
        std::shuffle(directions.begin(), directions.end(), Field::rng);

        bool moved = false;
        for (auto dir : directions) {
            sf::Vector2i new_pos = pos + dir;
            if (!is_valid_position(new_pos)) continue;
            auto& target = get_cell(new_pos.x, new_pos.y);
            if (target.getType() == CellType::Blocked ||
                target.getType() == CellType::Enemy ||
                target.getType() == CellType::Building ||
                target.getType() == CellType::Tower) // <-- Враги тоже не ходят на башни
            {
                continue;
            }
            if (target.getType() == CellType::Player) {
                player->change_health(-e->get_damage());
                moved = true;
                break;
            }
            get_cell(pos.x, pos.y).clear();
            target.setType(CellType::Enemy);
            target.setEnemy(e);
            if (target.getTrap()) {
                Trap* t = target.getTrap();
                std::cout << "Enemy at (" << new_pos.x << ", " << new_pos.y
                          << ") triggered a trap for " << t->getDamage() << " damage!\n";
                // Наносим урон (это может убить врага)
                damageEnemyAt(new_pos, t->getDamage());
                // Удаляем ловушку
                removeTrapAt(new_pos);
            }

            moved = true;
            break;
        }
    }
}

void Field::process_buildings() {
    for (auto b : buildings) {
        b->tick();
        if (b->is_ready_to_spawn()) {
            sf::Vector2i pos{-1, -1};
            for (int y = 0; y < rows; ++y) {
                for (int x = 0; x < cols; ++x) {
                    if (grid[y][x].getBuilding() == b) {
                        pos = {x, y};
                        break;
                    }
                }
                if (pos.x >= 0) break;
            }
            if (pos.x >= 0) {
                spawn_enemy_near_building(pos, b->get_enemy_health(), b->get_enemy_damage());
            }
            b->reset_timer();
        }
    }
}

void Field::spawn_enemy_near_building(sf::Vector2i building_pos, int e_health, int e_damage) {
    std::vector<sf::Vector2i> directions = {{0,-1}, {0,1}, {-1,0}, {1,0}};
    std::shuffle(directions.begin(), directions.end(), Field::rng);

    for (auto dir : directions) {
        sf::Vector2i new_pos = building_pos + dir;
        if (is_valid_position(new_pos) && get_cell(new_pos.x, new_pos.y).getType() == CellType::Empty) {
            Enemy* new_e = new Enemy(e_health, e_damage);
            enemies.push_back(new_e);
            auto& cell = get_cell(new_pos.x, new_pos.y);
            cell.setType(CellType::Enemy);
            cell.setEnemy(new_e);
            return;
        }
    }
}

bool Field::is_valid_position(sf::Vector2i pos) const {
    return pos.x >= 0 && pos.x < cols && pos.y >= 0 && pos.y < rows;
}

bool Field::is_game_over() const {
    return player && player->get_health() <= 0;
}

void Field::damageEnemyAt(sf::Vector2i pos, int damage) {
    if (!is_valid_position(pos)) return;
    auto& cell = get_cell(pos.x, pos.y);
    if (cell.getType() != CellType::Enemy) return;

    Enemy* e = cell.getEnemy();
    e->change_health(-damage);

    if (e->get_health() <= 0) {
        cell.clear(); // Очищаем клетку
        // Находим и удаляем врага из главного списка
        auto it = std::find(enemies.begin(), enemies.end(), e);
        if (it != enemies.end()) {
            delete *it;
            enemies.erase(it);
        }
        player->change_score(10);
        player->incrementKillCount(); // <-- НОВОЕ
        std::cout << "Player destroyed enemy! +10 pts. Kills: " << player->getKillCount() << "\n";
    }
}

void Field::damageBuildingAt(sf::Vector2i pos, int damage) {
    if (!is_valid_position(pos)) return;
    auto& cell = get_cell(pos.x, pos.y);
    if (cell.getType() != CellType::Building) return;

    EnemyBuilding* b = cell.getBuilding();
    b->change_health(-damage);

    if (b->get_health() <= 0) {
        cell.clear(); // Очищаем клетку
        // Находим и удаляем здание из главного списка
        auto it = std::find(buildings.begin(), buildings.end(), b);
        if (it != buildings.end()) {
            delete *it;
            buildings.erase(it);
        }
        player->change_score(25); // Больше очков за здание
        std::cout << "Player destroyed building! +25 pts.\n";
    }
}

void Field::process_towers() {
    sf::Vector2i playerPos = find_player_position();
    if (playerPos.x < 0) return; // Игрока нет на поле

    for (auto t : towers) {
        t->tick(); // Обновляем таймер перезарядки
        if (!t->isReady()) continue; // Башня не готова стрелять

        sf::Vector2i towerPos = find_tower_position(t);
        if (towerPos.x < 0) continue; // Башня не найдена?

        // Проверяем Манхэттенское расстояние
        int distance = std::abs(playerPos.x - towerPos.x) + std::abs(playerPos.y - towerPos.y);

        if (distance <= t->getRange()) {
            // Игрок в радиусе атаки
            std::cout << "Tower at (" << towerPos.x << ", " << towerPos.y
                      << ") shot player for " << t->getDamage() << " damage!\n";
            player->change_health(-t->getDamage());
            t->resetCooldown(); // Башня уходит на перезарядку
        }
    }
}

void Field::addTrap(Trap* trap, sf::Vector2i pos) {
    if (!is_valid_position(pos)) {
        delete trap; // Удаляем, если позиция невалидна
        return;
    }
    traps.push_back(trap);
    get_cell(pos.x, pos.y).setTrap(trap);
}

void Field::removeTrapAt(sf::Vector2i pos) {
    if (!is_valid_position(pos)) return;

    Cell& cell = get_cell(pos.x, pos.y);
    Trap* t = cell.getTrap();
    if (!t) return;

    cell.setTrap(nullptr); // Убираем указатель из клетки

    // Находим и удаляем ловушку из вектора владения
    auto it = std::find(traps.begin(), traps.end(), t);
    if (it != traps.end()) {
        delete *it;
        traps.erase(it);
    }
}

sf::Vector2i Field::find_tower_position(EnemyTower* t) const {
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if (grid[y][x].getTower() == t) {
                return {x, y};
            }
        }
    }
    return {-1, -1};
}