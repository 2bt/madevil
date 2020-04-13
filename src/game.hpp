#pragma once


#include "fx.hpp"
#include "math.hpp"
#include "enemy.hpp"
#include "hero.hpp"
#include <vector>
#include <string>



enum { TILE_SIZE = 16 };


inline bool is_solid(char t) {
    return t == '0';
}


class Game : public fx::App {
public:
    bool init() override;
    void update() override;
    void key(int code) override;

    float collision(Box const& box, Axis axis) const;
    char  tile_at(int col, int row) const;
    bool  check_sight(float x1, float y1, float x2, float y2) const;
    bool  can_see_hero(float x, float y) const {
        Box b = m_hero.box();
        return check_sight(x, y, b.x + b.w * 0.5, b.y + b.y * 0.5);
    }

private:
    std::vector<std::string> m_tiles;
    std::vector<Enemy::Ptr>  m_enemies;
    Hero                     m_hero = { *this };
};
