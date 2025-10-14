#include "Field.h"
#include <random>
#include <algorithm>

std::mt19937 Field::rng(std::random_device{}());

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
      player(other.player), enemies(std::move(other.enemies)), buildings(std::move(other.buildings)) {
    other.player = nullptr;
    other.rows = 0;
    other.cols = 0;
}

Field& Field::operator=(const Field& other) {
    if (this != &other) {
        for (auto e : enemies) delete e;
        for (auto b : buildings) delete b;
        enemies.clear();
        buildings.clear();
        player = nullptr;

        rows = other.rows;
        cols = other.cols;
        deep_copy(other);
    }
    return *this;
}

Field& Field::operator=(Field&& other) noexcept {
    if (this != &other) {
        for (auto e : enemies) delete e;
        for (auto b : buildings) delete b;
        enemies.clear();
        buildings.clear();
        delete player;

        rows = other.rows;
        cols = other.cols;
        grid = std::move(other.grid);
        player = other.player;
        enemies = std::move(other.enemies);
        buildings = std::move(other.buildings);

        other.rows = 0;
        other.cols = 0;
        other.player = nullptr;
    }
    return *this;
}

Field::~Field() {
    for (auto e : enemies) delete e;
    for (auto b : buildings) delete b;
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

void Field::initialize(Player* p, const std::vector<sf::Vector2i>& blocked_positions,
                       const std::vector<sf::Vector2i>& slowing_positions,
                       const std::vector<std::pair<sf::Vector2i, EnemyBuilding*>>& building_positions,
                       const std::vector<std::pair<sf::Vector2i, Enemy*>>& initial_enemies) {
    player = p;
    for (auto pos : blocked_positions) {
        get_cell(pos.x, pos.y).setType(CellType::Blocked);
    }
    for (auto pos : slowing_positions) {
        get_cell(pos.x, pos.y).setProperty(CellProperty::Slowing);
    }
    for (auto& pair : building_positions) {
        auto pos = pair.first;
        auto b = pair.second;
        buildings.push_back(b);
        auto& cell = get_cell(pos.x, pos.y);
        cell.setType(CellType::Building);
        cell.setBuilding(b);
    }
    for (auto& pair : initial_enemies) {
        auto pos = pair.first;
        auto e = pair.second;
        enemies.push_back(e);
        auto& cell = get_cell(pos.x, pos.y);
        cell.setType(CellType::Enemy);
        cell.setEnemy(e);
    }
    auto& start_cell = get_cell(0, 0);
    start_cell.setType(CellType::Player);
    start_cell.setPlayer(player);
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
    if (target.getType() == CellType::Blocked || target.getType() == CellType::Building) return false;
    if (target.getType() == CellType::Enemy) {
        Enemy* e = target.getEnemy();
        e->change_health(-player->get_damage());
        if (e->get_health() <= 0) {
            target.clear();
            auto it = std::find(enemies.begin(), enemies.end(), e);
            if (it != enemies.end()) {
                delete *it;
                enemies.erase(it);
            }
            player->change_score(10);
        }
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

    for (auto e : enemies) {
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
            if (target.getType() == CellType::Blocked || target.getType() == CellType::Enemy || target.getType() == CellType::Building) continue;
            if (target.getType() == CellType::Player) {
                player->change_health(-e->get_damage());
                moved = true;
                break;
            }
            get_cell(pos.x, pos.y).clear();
            target.setType(CellType::Enemy);
            target.setEnemy(e);
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