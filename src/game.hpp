#pragma once


#include "fx.hpp"
#include <vector>
#include <string>


inline float clamp(float x, float min, float max) {
    return std::max(min, std::min(max, x));
}

enum {
    TILE_SIZE = 16
};


enum class Axis { X, Y };

struct Box {
    float x, y, w, h;
    float overlap(Box const& rhs, Axis axis) const {
        if (x >= rhs.x + rhs.w
        || y >= rhs.y + rhs.h
        || x + w <= rhs.x
        || y + h <= rhs.y) return 0;
        float s, t;
        if (axis == Axis::X) {
            s = rhs.x + rhs.w - x;
            t = rhs.x - w - x;
        }
        else {
            s = rhs.y + rhs.h - y;
            t = rhs.y - h - y;
        }
        return std::abs(s) < std::abs(t) ? s : t;
    }
};




class Game;


class Player {
public:
    Player(Game& game) : m_game(game) {}
    enum Type { Male, Female };

    void update();
    void draw();

//private:
    Game& m_game;
    float m_x        = 50;
    float m_y        = 65;
    float m_vx       = 0;
    float m_vy       = 0;
    bool  m_airborne = true;
    int   m_dir      = 1;
    int   m_tick     = 0;
    Type  m_type     = Male;
};


class Game : public fx::App {
public:
    bool init() override;
    void update() override;
    void key(int code) override;

    float collision(Box const& box, Axis axis) const;

    char get_tile_at(int col, int row) const {
        if (col < 0 || row < 0 ||
            row >= (int) m_tiles.size() ||
            col >= (int) m_tiles[row].size()) return '0';
        return m_tiles[row][col];
    }

private:
    std::vector<std::string> m_tiles;
    Player                   m_player = { *this };
};
