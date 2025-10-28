#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>

class Player;
class Field;

class ISpell {
public:
    virtual ~ISpell() = default;

    // Информация для UI
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::unique_ptr<ISpell> clone() const = 0;
    // Максимальная дальность (Манхэттенское расстояние)
    virtual int getRange() const = 0;
    virtual void setRange(int range) {}
    virtual int getDamage() const { return 0; } // Используется для TrapSpell
    virtual void setDamage(int damage) {} // Используется для TrapSpell

    // Вспомогательный метод для определения типа (BuffSpell - это "улучшение")
    virtual bool isBuffSpell() const { return false; }
    /**
     * @brief Применяет заклинание.
     * @param player Игрок, кастующий заклинание.
     * @param field Игровое поле.
     * @param targetPos Целевая клетка.
     * @return true, если заклинание было успешно применено (и ход должен быть потрачен).
     * @return false, если применение не удалось (неверная цель, вне радиуса) - ход не тратится.
     */
    virtual bool use(Player& player, Field& field, sf::Vector2i targetPos) = 0;
};