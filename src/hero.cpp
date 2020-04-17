#include "hero.hpp"
#include "game.hpp"


Box Hero::box() const {
    if (m_airborne) return { m_x - 6, m_y - 20, 12, 20};
    return { m_x - 6, m_y - 23, 12, 23};
}


void Hero::update() {

    enum {
        RUN_UP = 20
    };

    int dx      = fx::input().x;
    bool jump   = fx::input().a && !fx::input().prev_a;
    bool attack = fx::input().b && !fx::input().prev_b;

    // attack
    if (!m_attacking && attack) {
        m_attacking = true;
        m_tick = 0;
        m_run  = 0;
    }
    if (m_attacking) {
        ++m_tick;
        if (!m_airborne) {
            m_vx = 0;
            if (m_tick >= 10 && m_tick < 25) m_vx = m_dir * 0.5;
        }
        if (m_tick >= 30) {
            m_attacking = false;
            m_tick      = 0;
        }
    }

    // walk
    if (!m_attacking && !m_airborne) {

        if (dx == 0) {
            m_tick = 0;
            m_run  = 0;
            m_vx   = 0;
        }
        else {
            if (m_dir != dx) {
                m_dir  = dx;
                m_tick = 0;
                m_run  = 0;
            }
            ++m_run;
            float speed = m_run < RUN_UP ? 3 : 5;
            m_tick += speed;
            m_vx = dx * speed * 0.25;
        }
    }

    m_x += m_vx;
    float f = m_game.collision(box(), Axis::X);
    m_x += f;
    if (f != 0) m_run = 0;

    // jump
    if (!m_attacking && !m_airborne && jump) {
        m_airborne = true;
        m_vy = -4.5;
    }

    // gravity
    m_vy += GRAVITY;

    m_y += clamp(m_vy, -MAX_VY, MAX_VY);
    float d = m_game.collision(box(), Axis::Y);
    if (d == 0) {
        if (!m_airborne) {
            // walking down a clip reduces speed
            m_vx  = m_dir * 0.25;
            m_run = 0;
        }
        m_airborne = true;
    }
    else {
        m_y += d;
        m_vy = 0;
        if (m_airborne && d < 0) {
            m_airborne = false;
            if (m_run < RUN_UP) m_run = 0;
            if (m_attacking) m_vx = 0;
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


        // dwarf walk
        { 48 * 0, 265, 48, 31 },
        { 48 * 1, 265, 48, 31 },
        { 48 * 2, 265, 48, 31 },
        { 48 * 1, 265, 48, 31 },
        // dwarf jump
        { 48 * 0, 265, 48, 31 },
        // dwarf attack
        { 48 * 3, 265, 48, 31 },
        { 48 * 4, 265, 48, 31 },
        { 48 * 5, 265, 48, 31 },

        // knight
        { 32 * 0, 304, 32, 32 },
        { 32 * 1, 304, 32, 32 },
        { 32 * 2, 304, 32, 32 },
        { 32 * 1, 304, 32, 32 },
        { 32 * 0, 304, 32, 32 },
        { 32 * 3, 304, 32, 32 },
        { 32 * 4, 304, 32, 32 },
        { 32 * 5, 304, 32, 32 },
    };

    int i;
    if (m_attacking) {
        if (m_tick < 6) i = 5;
        else if (m_tick < 13) i = 6;
        else i = 7;
    }
    else if (m_airborne) i = 4;
    else i = (m_tick + 30) / 40 % 4;


    if (m_type == Female) i += 5;
    if (m_type == Dwarf) i += 10;
    if (m_type == Knight) i += 18;

    fx::Rect frame = frames[i];
    fx::draw_sprite(m_x - frame.w / 2, m_y - 31, frame, m_dir < 0);

//    fx::set_color(200, 200, 100, 100);
//    Box b = box();
//    fx::draw_rectangle(false, b.x, b.y, b.w, b.h);
}
