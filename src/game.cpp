#include "game.hpp"
#include <fstream>


class Knight : public Enemy {
public:
    Knight(Game& game, float x, float y)
        : Enemy(game, x, y) {}

    void update() override {
        printf("%d\n", can_see_hero());
    }

    void draw() override {
//        fx::set_color(200, 200, 100, 100);
//        fx::draw_rectangle(false, m_x - 6, m_y - 23, 12, 23);

        static fx::Rect frames[] = {
            {  0, 72, 32, 32 },
            { 32, 72, 32, 32 },
            { 64, 72, 32, 32 },
            { 32, 72, 32, 32 },
        };
        fx::draw_sprite(m_x - 16, m_y - 32, frames[0], m_dir < 0);
    }

private:
    int m_dir = -1;
};


bool Game::init() {

    // load map
    std::ifstream file("assets/map.txt");
    if (!file) {
        LOG_ERROR("cannot open map");
        return false;
    }
    std::string line;
    for (int row = 0; std::getline(file, line); ++row) {
        m_tiles.push_back(line);
        for (int col = 0; col < (int) line.size(); ++col) {
            float x = col * TILE_SIZE + TILE_SIZE / 2;
            float y = row * TILE_SIZE + TILE_SIZE;
            switch (line[col]) {
            case '@': m_hero.init(x, y); break;
            case 'K': m_enemies.push_back(std::make_unique<Knight>(*this, x, y)); break;
            default: break;
            }
        }
    }

    return true;
}


void Game::update() {

    m_hero.update();


    // draw
    fx::set_color(68, 36, 52);
    fx::draw_rectangle(true, 0, 0, WIDTH, HEIGHT);


    // map
    for (int y = 0; y < 14; ++y)
    for (int x = 0; x < 24; ++x) {
        if (is_solid(tile_at(x, y))) {
            fx::draw_sprite(x * TILE_SIZE, y * TILE_SIZE, { 0, 240, 16, 16 });
        }
    }

    m_hero.draw();
    for (auto const& e : m_enemies) e->draw();

    fx::printf(8, 8, "MADEVIL");
}


char Game::tile_at(int col, int row) const {
    if (col < 0 || row < 0 ||
        row >= (int) m_tiles.size() ||
        col >= (int) m_tiles[row].size()) return '0';
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

        char t = tile_at(x, y);
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
    if (code == 30) m_hero.m_type = Hero::Male;
    if (code == 31) m_hero.m_type = Hero::Female;
}
