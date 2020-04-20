#pragma once


#include <memory>
#include "math.hpp"


class Game;


class Enemy {
public:
    using Ptr = std::unique_ptr<Enemy>;

    Enemy(Game& game, float x, float y)
        : m_game(game), m_x(x), m_y(y) {}
    virtual ~Enemy() {}

    virtual void update() = 0;
    virtual void draw() = 0;
    virtual Box box() const = 0;
    virtual void hit(int dir) {}
    bool is_alive() const { return m_alive; }
    bool can_see_hero() const;

protected:
    Game& m_game;
    float m_x;
    float m_y;
    bool  m_alive = true;
};

