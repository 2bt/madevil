#pragma once

#include <cstdio>


#define LOG_ERROR(fmt, ...) fprintf(stderr, "ERROR - " fmt "\n", ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  fprintf(stderr, "WARN  - " fmt "\n", ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  fprintf(stderr, "INFO  - " fmt "\n", ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) fprintf(stderr, "DEBUG - " fmt "\n", ##__VA_ARGS__)


enum {
    WIDTH     = 384,
    HEIGHT    = 216,
//    480
//    270
    TILE_SIZE = 16,
};


namespace fx {

    struct App {
        virtual ~App() {}
        virtual bool init() { return true; }
        virtual void free() {}
        virtual void key(int code) {}
        virtual void update() = 0;
    };
    struct Rect {
        int x, y, w, h;
    };
    enum {
        FLIP_NONE       = 0,
        FLIP_HORIZONTAL = 1,
        FLIP_VERTICAL   = 2,
    };
    struct Input {
        int  x, y, prev_x, prev_y;
        bool a, b, prev_a, prev_b;
    };

    void draw_tile(float x, float y, int t);
    void draw_sprite(float x, float y, Rect const& rect);
    void draw_sprite(float x, float y, Rect const& rect, int flip, float ang = 0);
    void set_color(int r, int g, int b, int a = 255);
    void draw_line(float x1, float y1, float x2, float y2);
    void draw_rectangle(bool fill, Rect const& rect);
    void set_font_color(int r, int g, int b);
    void put_char(float x, float y, char c);
    void print(float x, float y, const char* str);
    void printf(float x, float y, const char* format, ...);

    Input const& input();
    void exit(int result = 0);
    int run(App& App);
}
