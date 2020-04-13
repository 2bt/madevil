#pragma once


enum {
    WIDTH  = 384,
    HEIGHT = 216,
};


namespace fx {

    struct App {
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

    void draw_sprite(int x, int y, Rect const& rect);
    void draw_sprite(int x, int y, Rect const& rect, int flip, float ang = 0);
    void set_color(int r, int g, int b, int a = 255);
    void draw_line(int x1, int y1, int x2, int y2);
    void draw_rectangle(bool fill, int x, int y, int w, int h);
    void set_font_color(int r, int g, int b);
    void put_char(int x, int y, char c);
    void print(int x, int y, const char* str);
    void printf(int x, int y, const char* format, ...);

    Input const& input();
    void exit(int result = 0);
    int run(App& App);
}
