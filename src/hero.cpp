#include "hero.hpp"
#include "game.hpp"


Box Hero::box() const {
    if (m_airborne) return { m_x - 6, m_y - 20, 12, 20};
    return { m_x - 6, m_y - 23, 12, 23};
}


void Hero::update() {

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
        float d = m_game.collision(box(), Axis::X);
        m_x += d;
    }


    if (!m_airborne) {
        if (fx::input().a && !fx::input().prev_a) {
            m_airborne = true;
            m_vy = -4.5;
        }
    }

    m_vy += 0.25;


    m_y += clamp(m_vy, -3.5, 3.5);
    {
        float d = m_game.collision(box(), Axis::Y);
        if (d == 0) m_airborne = true;
        else {
            m_y += d;
            m_vy = 0;
            if (d < 0) m_airborne = false;
        }
    }
}


void Hero::draw() {
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

    fx::draw_sprite(m_x - 8, m_y - 31, frames[i], m_dir < 0);

//    fx::set_color(200, 200, 100, 100);
//    Box b = box();
//    fx::draw_rectangle(false, b.x, b.y, b.w, b.h);
}
