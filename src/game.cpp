#include "game.hpp"
#include <fstream>
#include <array>


class Knight : public Enemy {
public:
    Knight(Game& game, float x, float y)
        : Enemy(game, x, y) {}

    void update() override {
        ++m_tick;
        if (m_tick == 0) m_dir = -m_dir;
        if (m_tick > 20) m_x += m_dir * 0.5;

        float f = m_game.collision(box(), Axis::X);
        if (f != 0) {
            m_x += f;
            m_tick = -20;
        }


        if (m_tick > 0 && m_game.collision({m_x - 1 + m_dir * 10, m_y, 2, 2}, Axis::Y) == 0) {
            m_tick = -20;
        }

        // gravity
        m_vy += GRAVITY;

        m_y += clamp(m_vy, -MAX_VY, MAX_VY);
        float d = m_game.collision(box(), Axis::Y);
        if (d != 0) {
            m_y += d;
            m_vy = 0;
        }

    }

    void draw() override {
//        fx::set_color(200, 200, 100, 100);
//        Box b = box();
//        fx::draw_rectangle(false, b.x, b.y, b.w, b.h);

        static fx::Rect frames[] = {
            {  0, 72, 32, 32 },
            { 32, 72, 32, 32 },
            { 64, 72, 32, 32 },
            { 32, 72, 32, 32 },
        };
        int i = (m_tick + 100) / 16 % 4;
        fx::draw_sprite(m_x - 16 - m_game.camera().x,
                        m_y - 32 - m_game.camera().y,
                        frames[i], m_dir < 0);
    }

    Box box() const override {
        return { m_x - 10, m_y - 30, 20, 30 };
    }

private:
    int   m_dir  = -1;
    int   m_tick = 0;
    float m_vy   = 0;
};


bool Game::init() {
    return load_map("assets/map.tmx");
}

bool Game::load_map(char const* name) {
    FILE* f = fopen(name, "r");
    if (!f) {
        LOG_ERROR("cannot open map");
        return false;
    }
    std::array<char, 1024> line;
    while (fgets(line.data(), line.size(), f)) {
        std::array<char, 1024> buf;
        float x, y, w, h;
        if (sscanf(line.data(), R"( <layer name="%[^"]")", buf.data()) == 1 &&
            std::string(buf.data()) == "background")
        {
            if (!fgets(line.data(), line.size(), f)) break; // skip line
            while (fgets(line.data(), line.size(), f)) {
                if (std::string(line.data()) == "</data>\n") break;
                m_tiles.emplace_back();
                auto& row = m_tiles.back();
                char* p = line.data();
                while (*p && *p != '\n') {
                    int t = std::strtol(p, &p, 10);
                    row.push_back(t - 1);
                    if (*p) p++;
                }
            }
            continue;
        }
        if (sscanf(line.data(), R"( <object id="%*[^"]" name="%[^"]" x="%f" y="%f" width="%f" height="%f")", buf.data(), &x, &y, &w, &h) == 5) {
            std::string name = buf.data();
            if (name == "camera-barrier") {
                m_camera_barriers.push_back({ x, y, w, h });
                continue;
            }

            x += w / 2;
            y += h;
            if (name == "hero") m_hero.init(x, y);
            if (name == "knight") m_enemies.push_back(std::make_unique<Knight>(*this, x, y));
            continue;
        }
    }
    fclose(f);


    // move camera to hero
    Box hb = m_hero.box();
    m_camera.x = hb.x + hb.w * 0.5 - m_camera.w * 0.5;
    m_camera.y = hb.y - m_camera.h * 0.5;
    for (int i = 0; i < 100; ++i) {
        if (!update_camera()) break;
    }

    return true;
}


bool Game::update_camera() {
    Box hb = m_hero.box();
    Box cb = m_camera;
    m_camera.x = hb.x + hb.w * 0.5 - m_camera.w * 0.5;
    if (!m_hero.is_airborne() || hb.y > cb.y + cb.h * 0.6) {
        m_camera.y = hb.y - m_camera.h * 0.5;
    }
    bool auto_move = false;
    for (Box const& b : m_camera_barriers) {
        if (b.w > b.h) {
            float d = m_camera.overlap(b, Axis::Y);
            m_camera.y += d;
            if (d != 0) auto_move = true;
        }
        else {
            float d = m_camera.overlap(b, Axis::X);
            m_camera.x += d;
            if (d != 0) auto_move = true;
        }
    }

    float speed = 1;
    m_camera.y = clamp(m_camera.y, cb.y - speed, cb.y + speed);
    m_camera.x = clamp(m_camera.x, cb.x - speed, cb.x + speed);

    return auto_move;
}


void Game::update() {

    // update hero
    m_hero.update();

    // update camera
    update_camera();

    // update enemies
    for (auto it = m_enemies.begin(); it != m_enemies.end();) {
        (*it)->update();
        if (!(*it)->is_alive()) {
            it = m_enemies.erase(it);
            continue;
        }
        ++it;
    }

///////////////////////////////// draw /////////////////////////////////

    fx::set_color(48, 52, 109);
    fx::draw_rectangle(true, { 0, 0, WIDTH, HEIGHT });

    {
        int x1 = std::floor(m_camera.x / TILE_SIZE);
        int x2 = std::floor((m_camera.x + m_camera.w + TILE_SIZE) / TILE_SIZE);
        int y1 = std::floor(m_camera.y / TILE_SIZE);
        int y2 = std::floor((m_camera.y + m_camera.h + TILE_SIZE) / TILE_SIZE);
        for (int y = y1; y <= y2; ++y)
        for (int x = x1; x <= x2; ++x) {
            int t = tile_at(x, y);
            if (t >= 0) fx::draw_tile(x * TILE_SIZE - m_camera.x, y * TILE_SIZE - m_camera.y, t);
        }
    }

    m_hero.draw();
    for (auto const& e : m_enemies) e->draw();

    fx::printf(8, 8, "MADEVIL");
}


int Game::tile_at(int col, int row) const {
    if (col < 0 || row < 0 ||
        row >= (int) m_tiles.size() ||
        col >= (int) m_tiles[row].size()) return 0;
    return m_tiles[row][col];
}


float Game::collision(Box const& box, Axis axis) const {

    int x1 = std::floor(box.x / TILE_SIZE);
    int x2 = std::floor((box.x + box.w + TILE_SIZE) / TILE_SIZE);
    int y1 = std::floor(box.y / TILE_SIZE);
    int y2 = std::floor((box.y + box.h + TILE_SIZE) / TILE_SIZE);

    float d = 0;

    Box tile_box = { 0, 0, TILE_SIZE, TILE_SIZE };
    for (int y = y1; y <= y2; ++y)
    for (int x = x1; x <= x2; ++x) {

        int t = tile_at(x, y);
        if (is_solid(t)) {

            tile_box.x = x * TILE_SIZE;
            tile_box.y = y * TILE_SIZE;
            float e = box.overlap(tile_box, axis);

            if (std::abs(e) > std::abs(d)) d = e;

//            if (t == '0') {
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

bool Game::check_sight(float x1, float y1, float x2, float y2) const {

    int xx1 = std::floor(x1 / TILE_SIZE);
    int yy1 = std::floor(y1 / TILE_SIZE);
    int xx2 = std::floor(x2 / TILE_SIZE);
    int yy2 = std::floor(y2 / TILE_SIZE);

    int sx = sign(xx2 - xx1);
    int sy = sign(yy2 - yy1);

    if (is_solid(tile_at(xx1, yy1))) return false;

    while (xx1 != xx2 && yy1 != yy2) {
        if (xx1 != xx2) xx1 += sx;
        if (yy1 != yy2) yy1 += sy;
        if (is_solid(tile_at(xx1, yy1))) return false;
    }
    return true;
}

void Game::key(int code) {
//    printf("key %d\n", code);
    if (code == 30) m_hero.m_type = Hero::Male;
    if (code == 31) m_hero.m_type = Hero::Female;
    if (code == 32) m_hero.m_type = Hero::Dwarf;
    if (code == 33) m_hero.m_type = Hero::Knight;
}
