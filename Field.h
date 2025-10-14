#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Cell.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include <random>

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

    void initialize(Player* p, const std::vector<sf::Vector2i>& blocked_positions,
                    const std::vector<sf::Vector2i>& slowing_positions,
                    const std::vector<std::pair<sf::Vector2i, EnemyBuilding*>>& building_positions,
                    const std::vector<std::pair<sf::Vector2i, Enemy*>>& initial_enemies);

    sf::Vector2i find_player_position() const;
    bool move_player(sf::Vector2i direction);
    void move_enemies();
    void process_buildings();
    void spawn_enemy_near_building(sf::Vector2i building_pos, int e_health, int e_damage);
    bool is_valid_position(sf::Vector2i pos) const;
    bool is_game_over() const;
};