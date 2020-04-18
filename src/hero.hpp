#pragma once


#include "math.hpp"


class Game;


class Hero {
public:
    Hero(Game& game) : m_game(game) {}
    enum Type { Male, Female, Dwarf, Knight };

    void init(float x, float y) {
        m_x         = x;
        m_y         = y;
        m_dir       = 1;
        m_vx        = 0;
        m_vy        = 0;
        m_airborne  = true;
        m_attacking = false;
        m_tick      = 0;
        m_run       = 0;
        m_type      = Knight;
    }
    void update();
    void draw();
    bool is_airborne() const { return m_airborne; }

    Box box() const;

    Type  m_type;
private:

    Game& m_game;
    float m_x;
    float m_y;
    int   m_dir;

    float m_vx;
    float m_vy;
    bool  m_airborne;
    bool  m_attacking;
    int   m_tick;
    int   m_run;
};
