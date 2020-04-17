#include "fx.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <array>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif


namespace fx {
namespace {

#include "font.hpp"

SDL_Window*   s_window;
SDL_Renderer* s_renderer;
SDL_Texture*  s_font_tex;
SDL_Texture*  s_sprite_tex;
SDL_Texture*  s_tile_tex;
Input         s_input;
bool          s_running;
int           s_result;


void init_font() {

    std::array<uint16_t, 16 * 6 * 8 * 8> data;
    for (int i = 0; i < (int) data.size(); ++i) {
        data[i] = (FONT[i / 8] & (1 << (7 - i % 8))) ? 0xffff : 0;
    }
    s_font_tex = SDL_CreateTexture(s_renderer, SDL_PIXELFORMAT_ARGB4444,
                                   SDL_TEXTUREACCESS_STATIC, 16 * 8, 6 * 8);
    if (!s_font_tex) {
        LOG_ERROR("cannot open font");
        s_running = false;
        s_result = 1;
        return;
    }
    SDL_UpdateTexture(s_font_tex, nullptr, data.data(), 2 * 16 * 8);
    SDL_SetTextureBlendMode(s_font_tex, SDL_BLENDMODE_BLEND);
}

void loop(void* arg) {
    App& app = *(App*) arg;

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT:
            s_running = false;
            break;

        case SDL_KEYDOWN:
            //if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) s_running = false;
            app.key(e.key.keysym.scancode);
            break;

        default: break;
        }
    }


    const Uint8* ks = SDL_GetKeyboardState(nullptr);

    s_input.prev_x = s_input.x;
    s_input.prev_y = s_input.y;
    s_input.prev_a = s_input.a;
    s_input.prev_b = s_input.b;
    s_input.x = !!ks[SDL_SCANCODE_RIGHT] - !!ks[SDL_SCANCODE_LEFT];
    s_input.y = !!ks[SDL_SCANCODE_DOWN] - !!ks[SDL_SCANCODE_UP];
    s_input.a = !!ks[SDL_SCANCODE_X];
    s_input.b = !!ks[SDL_SCANCODE_C];

    SDL_SetRenderDrawColor(s_renderer, 0, 0, 0, 0);
    SDL_RenderClear(s_renderer);

    app.update();

    SDL_RenderPresent(s_renderer);
}


SDL_Texture* load_tex(char const* name) {
    SDL_Texture* tex = IMG_LoadTexture(s_renderer, name);
    if (!tex) LOG_ERROR("cannot open %s", name);
    return tex;
}


} // namespace

void exit(int res) {
    s_result = res;
    s_running = false;
}


int run(App& app) {
    s_running = true;
    s_result = 0;

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);

    s_window = SDL_CreateWindow("MADEVIL",
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
#ifdef __EMSCRIPTEN__
                                WIDTH * 2, HEIGHT * 2, 0);
#else
                                WIDTH * 2, HEIGHT * 2, SDL_WINDOW_RESIZABLE);
#endif

    s_renderer = SDL_CreateRenderer(s_window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetLogicalSize(s_renderer, WIDTH, HEIGHT);


    s_sprite_tex = load_tex("assets/sprite.png");
    s_tile_tex   = load_tex("assets/tile.png");
    if (!s_sprite_tex || !s_tile_tex) {
        s_running = false;
        s_result = 1;
    }

    init_font();

    if (!app.init()) {
        LOG_ERROR("app.init failed");
        s_running = false;
        s_result = 1;
    }

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(loop, &app, -1, 1);
#else
    while (s_running) loop(&app);
#endif

    app.free();
    SDL_DestroyTexture(s_tile_tex);
    SDL_DestroyTexture(s_sprite_tex);
    SDL_DestroyTexture(s_font_tex);
    SDL_DestroyRenderer(s_renderer);
    SDL_DestroyWindow(s_window);
    SDL_Quit();
    IMG_Quit();
    return s_result;
}

Input const& input() { return s_input; }

void draw_tile(int x, int y, int t) {
    SDL_Rect src = {
        t % 16 * TILE_SIZE,
        t / 16 * TILE_SIZE,
        TILE_SIZE,
        TILE_SIZE
    };
    SDL_Rect dst = { x, y, TILE_SIZE, TILE_SIZE };
    SDL_RenderCopy(s_renderer, s_tile_tex, &src, &dst);
}

void draw_sprite(int x, int y, Rect const& rect) {
    SDL_Rect src = { rect.x, rect.y, rect.w, rect.h };
    SDL_Rect dst = { x, y, rect.w, rect.h };
    SDL_RenderCopy(s_renderer, s_sprite_tex, &src, &dst);
}

void draw_sprite(int x, int y, Rect const& rect, int flip, float ang) {
    SDL_Rect src = { rect.x, rect.y, rect.w, rect.h };
    SDL_Rect dst = { x, y, rect.w, rect.h };
    SDL_RenderCopyEx(s_renderer, s_sprite_tex, &src, &dst, ang, nullptr, (SDL_RendererFlip) flip);
}

void set_color(int r, int g, int b, int a) {
    SDL_SetRenderDrawColor(s_renderer, r, g, b, a);
}

void draw_line(int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(s_renderer, x1, y1, x2, y2);
}

void draw_rectangle(bool fill, int x, int y, int w, int h) {
    SDL_Rect r = { x, y, w, h };
    if (fill) SDL_RenderFillRect(s_renderer, &r);
    else      SDL_RenderDrawRect(s_renderer, &r);
}

void set_font_color(int r, int g, int b) {
    SDL_SetTextureColorMod(s_font_tex, r, g, b);
}

void put_char(int x, int y, char c) {
    if (c < 32) return;
    SDL_Rect src = { c % 16 * 8, (c - 32) / 16 * 8, 8, 8 };
    SDL_Rect dst = { x, y, 8, 8 };
    SDL_RenderCopy(s_renderer, s_font_tex, &src, &dst);
}

void print(int x, int y, const char* str) {
    while (*str) {
        put_char(x, y, *str);
        ++str;
        x += 8;
    }
}

void printf(int x, int y, const char* format, ...) {
    va_list args;
    va_start(args, format);
    static std::array<char, 1024> line;
    vsnprintf(line.data(), line.size(), format, args);
    va_end(args);
    print(x, y, line.data());
}

} // namespace
