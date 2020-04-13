#include "game.hpp"
#include <cmath>
#include <fstream>


void Player::update() {

    if (!m_airborne) {
        int dx = fx::input().x;
        if (dx == 0) {
            m_tick = 7;
            m_vx   = 0;
        }
        else {
            if (m_dir != dx) {
                m_dir = dx;
                m_tick = 0;
            }
            float speed = m_tick < 60 ? 3 : 5;
            m_tick += speed;
            m_vx = dx * speed * 0.25;
        }
    }


    m_x += m_vx;
    {
        float d = m_game.collision({ m_x + 1, m_y + 8, 14, 23}, Axis::X);
        m_x += d;
    }


    if (m_airborne) {
        
    }
    else {
        if (fx::input().a && !fx::input().prev_a) {
            m_airborne = true;
            m_vy = -4.5;
        }
    }

    m_vy += 0.25;


    m_y += clamp(m_vy, -3.5, 3.5);
//    m_y += fx::input().y;
    {
        float d = m_game.collision({ m_x + 1, m_y + 8, 14, 23}, Axis::Y);
        if (d == 0) m_airborne = true;
        else {
            m_y += d;
            m_vy = 0;
            if (d < 0) m_airborne = false;
        }
    }
}


void Player::draw() {
    static fx::Rect frames[] = {
        {  0, 0, 16, 32 },
        { 16, 0, 16, 32 },
        { 32, 0, 16, 32 },
        { 16, 0, 16, 32 },
        { 48, 4, 16, 28 },

        {  0, 32, 16, 32 },
        { 16, 32, 16, 32 },
        { 32, 32, 16, 32 },
        { 16, 32, 16, 32 },
        { 48, 36, 16, 28 },
    };
    int i = m_tick / 40 % 4;
    if (m_airborne) i = 4;
    if (m_type == Female) i += 5;

    fx::draw_sprite(m_x, m_y, frames[i], m_dir < 0);

//    fx::set_color(200, 200, 100, 100);
//    fx::draw_rectangle(false, m_x + 1, m_y + 8, 14, 23);
}



bool Game::init() {

    // load map
    std::ifstream file("assets/map.txt");
    if (!file) printf("can't open map\n");
    std::string line;
    for (int row = 0; std::getline(file, line); ++row) {
        m_tiles.push_back(line);
        for (int col = 0; col < (int) line.size(); ++col) {
            char t = line[col];
            if (t == '@') {
                m_player.m_x = col * TILE_SIZE + TILE_SIZE / 2 - 7;
                m_player.m_y = row * TILE_SIZE + TILE_SIZE - 31;
            }
        }
    }

    return true;
}


void Game::update() {

    m_player.update();


    // draw
    fx::set_color(68, 36, 52);
    fx::draw_rectangle(true, 0, 0, WIDTH, HEIGHT);


    // map
    for (int y = 0; y < 14; ++y) {
        for (int x = 0; x < 24; ++x) {
            int c = get_tile_at(x, y);
            if (c == '0') {
                fx::draw_sprite(x * TILE_SIZE, y * TILE_SIZE, { 0, 240, 16, 16 });
            }
        }
    }

    m_player.draw();


    fx::printf(8, 8, "MADEVIL");
}

float Game::collision(Box const& box, Axis axis) const {

    int x1 = std::floor(box.x / TILE_SIZE);
    int x2 = std::floor((box.x + box.w + TILE_SIZE) / TILE_SIZE);
    int y1 = std::floor(box.y / TILE_SIZE);
    int y2 = std::floor((box.y + box.h + TILE_SIZE) / TILE_SIZE);

    Box tile_box = { 0, 0, TILE_SIZE, TILE_SIZE };

    float d = 0;

    for (int y = y1; y <= y2; ++y)
    for (int x = x1; x <= x2; ++x) {

        char t = get_tile_at(x, y);
//        if (t == TILE_WALL || t == TILE_BOARD) {
        if (t == '0') {

            tile_box.x = x * TILE_SIZE;
            tile_box.y = y * TILE_SIZE;
            float e = box.overlap(tile_box, axis);

//            if (t == '0') {
                if (std::abs(e) > std::abs(d)) d = e;
//            }
//            else if (t == TILE_BOARD) { // jump though
//                if (axis == Axis::Y && vel > 0 && e < 0 && -e <= vel + 0.001f) {
//                    d = e;
//                }
//            }
        }
    }

    return d;
}

void Game::key(int code) {
    if (code == 30) m_player.m_type = Player::Male;
    if (code == 31) m_player.m_type = Player::Female;
}
