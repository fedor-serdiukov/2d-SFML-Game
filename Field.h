#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Cell.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include <random>

struct FieldContent {
    std::vector<sf::Vector2i> blocked;
    std::vector<sf::Vector2i> slowing;
    std::vector<std::pair<sf::Vector2i, EnemyBuilding*>> buildings;
    std::vector<std::pair<sf::Vector2i, Enemy*>> initial_enemies;
};

class Field {
private:
    int rows;
    int cols;
    std::vector<std::vector<Cell>> grid;
    Player* player;
    std::vector<Enemy*> enemies;
    std::vector<EnemyBuilding*> buildings;
    void deep_copy(const Field& other);
    static std::mt19937 rng;
public:
    Field(int r, int c);
    Field(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(const Field& other);
    Field& operator=(Field&& other) noexcept;
    ~Field();

    int get_rows() const;
    int get_cols() const;
    Cell& get_cell(int x, int y);
    const Cell& get_cell(int x, int y) const;
    sf::Vector2i random_pos(int max_x, int max_y);
    FieldContent generate_random_content(int blocked_count, int slowing_count, int building_count, int enemy_count, int COLS, int ROWS);
    void initialize(Player* p, const FieldContent& content);

    sf::Vector2i find_player_position() const;
    bool move_player(sf::Vector2i direction);
    void move_enemies();
    void process_buildings();
    void spawn_enemy_near_building(sf::Vector2i building_pos, int e_health, int e_damage);
    bool is_valid_position(sf::Vector2i pos) const;
    bool is_game_over() const;
};