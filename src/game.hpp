#pragma once


#include "fx.hpp"
#include "math.hpp"
#include "enemy.hpp"
#include "hero.hpp"
#include <vector>
#include <string>



constexpr float GRAVITY = 0.25;
constexpr float MAX_VY  = 3.5;


inline bool is_solid(int t) {
    return t >= 0 && t < 16;
}


class Game : public fx::App {
public:
    bool init() override;
    void update() override;
    void key(int code) override;

    float collision(Box const& box, Axis axis) const;
    int   tile_at(int col, int row) const;
    bool  check_sight(float x1, float y1, float x2, float y2) const;
    bool  can_see_hero(float x, float y) const {
        Box b = m_hero.box();
        return check_sight(x, y, b.x + b.w * 0.5, b.y + b.h * 0.5);
    }

    Box const& camera() const { return m_camera; }
    std::vector<Enemy::Ptr> const& enemies() { return m_enemies; }

    void draw_debug_box(Box const& b) {
        if (!m_debug) return;
        fx::draw_rectangle(false, b.x - camera().x, b.y - camera().y, b.w, b.h);
    }

private:
    bool load_map(char const* name);
    bool update_camera();


    std::vector<std::vector<int>> m_tiles;
    std::vector<Box>              m_camera_barriers;

    std::vector<Enemy::Ptr>       m_enemies;
    Hero                          m_hero   = { *this };
    Box                           m_camera = { 0, 0, WIDTH, HEIGHT };

    bool                          m_debug = true;
};
