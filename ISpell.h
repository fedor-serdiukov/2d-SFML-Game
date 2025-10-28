#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>
#include <memory>

class Player;
class Field;

class ISpell {
public:
    virtual ~ISpell() = default;
    virtual std::unique_ptr<ISpell> clone() const = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual int getRange() const = 0;
    virtual bool use(Player& player, Field& field, sf::Vector2i targetPos) = 0;
    virtual bool isBuffSpell() const { return false; }
    virtual void setRange(int range) {}
    virtual int getDamage() const { return 0; }
    virtual void setDamage(int damage) {}
    virtual void setSummonsCount(int count) {}
};