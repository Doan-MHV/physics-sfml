#ifndef GAMEPHYSICS_GLDRAW_HPP
#define GAMEPHYSICS_GLDRAW_HPP

#include <OpenGL/gl.h>
#include <SDL3/SDL.h>

#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <iostream>

namespace gl_draw {

struct Color {
    float r{1.0f}, g{1.0f}, b{1.0f}, a{1.0f};
};

// Standard palette for physics visualizations
inline constexpr Color kBgDark{0.08f, 0.09f, 0.12f, 1.0f};
inline constexpr Color kGridLine{0.16f, 0.18f, 0.23f, 1.0f};
inline constexpr Color kAxisX{0.85f, 0.30f, 0.30f, 1.0f};   // Red (+X)
inline constexpr Color kAxisY{0.30f, 0.80f, 0.40f, 1.0f};   // Green (+Y)
inline constexpr Color kCyan{0.25f, 0.75f, 0.95f, 1.0f};    // Velocity / primary body
inline constexpr Color kOrange{0.90f, 0.45f, 0.20f, 1.0f};  // Secondary body
inline constexpr Color kYellow{0.98f, 0.82f, 0.25f, 1.0f};  // Force / acceleration
inline constexpr Color kWhite{0.95f, 0.96f, 0.98f, 1.0f};
inline constexpr Color kGray{0.55f, 0.58f, 0.65f, 1.0f};

// ============================================================================
// Built-in 5x7 ASCII Bitmap Font (Characters 32 ' ' to 126 '~')
// Each character is 5 columns wide by 7 rows tall (bit 0 = top row, bit 6 = bottom row)
// ============================================================================
inline const unsigned char* getGlyph5x7(char c) {
    static const unsigned char kFont[95][5] = {
        {0x00, 0x00, 0x00, 0x00, 0x00},  // ' '
        {0x00, 0x00, 0x5F, 0x00, 0x00},  // '!'
        {0x00, 0x07, 0x00, 0x07, 0x00},  // '"'
        {0x14, 0x7F, 0x14, 0x7F, 0x14},  // '#'
        {0x24, 0x2A, 0x7F, 0x2A, 0x12},  // '$'
        {0x23, 0x13, 0x08, 0x64, 0x62},  // '%'
        {0x36, 0x49, 0x55, 0x22, 0x50},  // '&'
        {0x00, 0x05, 0x03, 0x00, 0x00},  // '''
        {0x00, 0x1C, 0x22, 0x41, 0x00},  // '('
        {0x00, 0x41, 0x22, 0x1C, 0x00},  // ')'
        {0x14, 0x08, 0x3E, 0x08, 0x14},  // '*'
        {0x08, 0x08, 0x3E, 0x08, 0x08},  // '+'
        {0x00, 0x50, 0x30, 0x00, 0x00},  // ','
        {0x08, 0x08, 0x08, 0x08, 0x08},  // '-'
        {0x00, 0x60, 0x60, 0x00, 0x00},  // '.'
        {0x20, 0x10, 0x08, 0x04, 0x02},  // '/'
        {0x3E, 0x51, 0x49, 0x45, 0x3E},  // '0'
        {0x00, 0x42, 0x7F, 0x40, 0x00},  // '1'
        {0x42, 0x61, 0x51, 0x49, 0x46},  // '2'
        {0x21, 0x41, 0x45, 0x4B, 0x31},  // '3'
        {0x18, 0x14, 0x12, 0x7F, 0x10},  // '4'
        {0x27, 0x45, 0x45, 0x45, 0x39},  // '5'
        {0x3C, 0x4A, 0x49, 0x49, 0x30},  // '6'
        {0x01, 0x71, 0x09, 0x05, 0x03},  // '7'
        {0x36, 0x49, 0x49, 0x49, 0x36},  // '8'
        {0x06, 0x49, 0x49, 0x29, 0x1E},  // '9'
        {0x00, 0x36, 0x36, 0x00, 0x00},  // ':'
        {0x00, 0x56, 0x36, 0x00, 0x00},  // ';'
        {0x08, 0x14, 0x22, 0x41, 0x00},  // '<'
        {0x14, 0x14, 0x14, 0x14, 0x14},  // '='
        {0x00, 0x41, 0x22, 0x14, 0x08},  // '>'
        {0x02, 0x01, 0x51, 0x09, 0x06},  // '?'
        {0x32, 0x49, 0x79, 0x41, 0x3E},  // '@'
        {0x7E, 0x11, 0x11, 0x11, 0x7E},  // 'A'
        {0x7F, 0x49, 0x49, 0x49, 0x36},  // 'B'
        {0x3E, 0x41, 0x41, 0x41, 0x22},  // 'C'
        {0x7F, 0x41, 0x41, 0x22, 0x1C},  // 'D'
        {0x7F, 0x49, 0x49, 0x49, 0x41},  // 'E'
        {0x7F, 0x09, 0x09, 0x09, 0x01},  // 'F'
        {0x3E, 0x41, 0x49, 0x49, 0x7A},  // 'G'
        {0x7F, 0x08, 0x08, 0x08, 0x7F},  // 'H'
        {0x00, 0x41, 0x7F, 0x41, 0x00},  // 'I'
        {0x20, 0x40, 0x41, 0x3F, 0x01},  // 'J'
        {0x7F, 0x08, 0x14, 0x22, 0x41},  // 'K'
        {0x7F, 0x40, 0x40, 0x40, 0x40},  // 'L'
        {0x7F, 0x02, 0x0C, 0x02, 0x7F},  // 'M'
        {0x7F, 0x04, 0x08, 0x10, 0x7F},  // 'N'
        {0x3E, 0x41, 0x41, 0x41, 0x3E},  // 'O'
        {0x7F, 0x09, 0x09, 0x09, 0x06},  // 'P'
        {0x3E, 0x41, 0x51, 0x21, 0x5E},  // 'Q'
        {0x7F, 0x09, 0x19, 0x29, 0x46},  // 'R'
        {0x46, 0x49, 0x49, 0x49, 0x31},  // 'S'
        {0x01, 0x01, 0x7F, 0x01, 0x01},  // 'T'
        {0x3F, 0x40, 0x40, 0x40, 0x3F},  // 'U'
        {0x1F, 0x20, 0x40, 0x20, 0x1F},  // 'V'
        {0x3F, 0x40, 0x38, 0x40, 0x3F},  // 'W'
        {0x63, 0x14, 0x08, 0x14, 0x63},  // 'X'
        {0x07, 0x08, 0x70, 0x08, 0x07},  // 'Y'
        {0x61, 0x51, 0x49, 0x45, 0x43},  // 'Z'
        {0x00, 0x7F, 0x41, 0x41, 0x00},  // '['
        {0x02, 0x04, 0x08, 0x10, 0x20},  // '\'
        {0x00, 0x41, 0x41, 0x7F, 0x00},  // ']'
        {0x04, 0x02, 0x01, 0x02, 0x04},  // '^'
        {0x40, 0x40, 0x40, 0x40, 0x40},  // '_'
        {0x00, 0x01, 0x02, 0x04, 0x00},  // '`'
        {0x20, 0x54, 0x54, 0x54, 0x78},  // 'a'
        {0x7F, 0x48, 0x44, 0x44, 0x38},  // 'b'
        {0x38, 0x44, 0x44, 0x44, 0x20},  // 'c'
        {0x38, 0x44, 0x44, 0x48, 0x7F},  // 'd'
        {0x38, 0x54, 0x54, 0x54, 0x18},  // 'e'
        {0x08, 0x7E, 0x09, 0x01, 0x02},  // 'f'
        {0x0C, 0x52, 0x52, 0x52, 0x3E},  // 'g'
        {0x7F, 0x08, 0x04, 0x04, 0x78},  // 'h'
        {0x00, 0x44, 0x7D, 0x40, 0x00},  // 'i'
        {0x20, 0x40, 0x44, 0x3D, 0x00},  // 'j'
        {0x7F, 0x10, 0x28, 0x44, 0x00},  // 'k'
        {0x00, 0x41, 0x7F, 0x40, 0x00},  // 'l'
        {0x7C, 0x04, 0x18, 0x04, 0x78},  // 'm'
        {0x7C, 0x08, 0x04, 0x04, 0x78},  // 'n'
        {0x38, 0x44, 0x44, 0x44, 0x38},  // 'o'
        {0x7C, 0x14, 0x14, 0x14, 0x08},  // 'p'
        {0x08, 0x14, 0x14, 0x18, 0x7C},  // 'q'
        {0x7C, 0x08, 0x04, 0x04, 0x08},  // 'r'
        {0x48, 0x54, 0x54, 0x54, 0x20},  // 's'
        {0x04, 0x3F, 0x44, 0x40, 0x20},  // 't'
        {0x3C, 0x40, 0x40, 0x20, 0x7C},  // 'u'
        {0x1C, 0x20, 0x40, 0x20, 0x1C},  // 'v'
        {0x3C, 0x40, 0x30, 0x40, 0x3C},  // 'w'
        {0x44, 0x28, 0x10, 0x28, 0x44},  // 'x'
        {0x0C, 0x50, 0x50, 0x50, 0x3C},  // 'y'
        {0x44, 0x64, 0x54, 0x4C, 0x44},  // 'z'
        {0x00, 0x08, 0x36, 0x41, 0x00},  // '{'
        {0x00, 0x00, 0x7F, 0x00, 0x00},  // '|'
        {0x00, 0x41, 0x36, 0x08, 0x00},  // '}'
        {0x10, 0x08, 0x08, 0x10, 0x08}   // '~'
    };
    if (c < 32 || c > 126) return kFont[0];
    return kFont[c - 32];
}

// Draws formatted text in WORLD coordinates (meters) at (worldX, worldY)
// Each "pixel" of the 5x7 font is pixelSizeMeters wide/tall (default 0.04m = 2 screen pixels)
inline void drawTextWorld(float worldX, float worldY, Color color, const char* fmt, ...) {
    char buf[256];
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    constexpr float s = 0.045f;  // Size of one bitmap dot in meters
    float cursorX = worldX;
    float cursorY = worldY;

    glColor4f(color.r, color.g, color.b, color.a);
    glBegin(GL_QUADS);
    for (const char* p = buf; *p; ++p) {
        if (*p == '\n') {
            cursorX = worldX;
            cursorY -= 9.0f * s;
            continue;
        }
        const unsigned char* glyph = getGlyph5x7(*p);
        for (int col = 0; col < 5; ++col) {
            unsigned char bits = glyph[col];
            for (int row = 0; row < 7; ++row) {
                if (bits & (1 << row)) {
                    // row 0 is top of character, row 6 is bottom
                    float px = cursorX + static_cast<float>(col) * s;
                    float py = cursorY + static_cast<float>(6 - row) * s;
                    glVertex2f(px, py);
                    glVertex2f(px + s, py);
                    glVertex2f(px + s, py + s);
                    glVertex2f(px, py + s);
                }
            }
        }
        cursorX += 6.0f * s;
    }
    glEnd();
}

inline void setup2DCartesianView(int windowWidth, int windowHeight, float pixelsPerUnit = 50.0f) {
    glViewport(0, 0, windowWidth, windowHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float halfW = (static_cast<float>(windowWidth) * 0.5f) / pixelsPerUnit;
    float halfH = (static_cast<float>(windowHeight) * 0.5f) / pixelsPerUnit;

    glOrtho(-halfW, halfW, -halfH, halfH, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

inline void clearScreen(Color bg = kBgDark) {
    glClearColor(bg.r, bg.g, bg.b, bg.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

inline void drawLine(float x1, float y1, float x2, float y2, Color color, float lineWidth = 1.5f) {
    glLineWidth(lineWidth);
    glColor4f(color.r, color.g, color.b, color.a);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

inline void drawArrow(float x1, float y1, float x2, float y2, Color color, float headLength = 0.30f) {
    drawLine(x1, y1, x2, y2, color, 2.0f);

    float dx = x2 - x1;
    float dy = y2 - y1;
    if (std::sqrt(dx * dx + dy * dy) < 1e-4f) return;

    float angle = std::atan2(dy, dx);
    constexpr float kHeadAngle = 0.45f;

    glColor4f(color.r, color.g, color.b, color.a);
    glBegin(GL_TRIANGLES);
    glVertex2f(x2, y2);
    glVertex2f(x2 - headLength * std::cos(angle - kHeadAngle), y2 - headLength * std::sin(angle - kHeadAngle));
    glVertex2f(x2 - headLength * std::cos(angle + kHeadAngle), y2 - headLength * std::sin(angle + kHeadAngle));
    glEnd();
}

inline void drawCircle(float cx, float cy, float radius, Color color, bool filled = true, int segments = 40) {
    constexpr float kTwoPi = 6.28318530718f;
    glColor4f(color.r, color.g, color.b, color.a);

    if (filled) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= segments; ++i) {
            float theta = kTwoPi * static_cast<float>(i) / static_cast<float>(segments);
            glVertex2f(cx + radius * std::cos(theta), cy + radius * std::sin(theta));
        }
        glEnd();
    } else {
        glLineWidth(1.5f);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < segments; ++i) {
            float theta = kTwoPi * static_cast<float>(i) / static_cast<float>(segments);
            glVertex2f(cx + radius * std::cos(theta), cy + radius * std::sin(theta));
        }
        glEnd();
    }
}

// Draws Cartesian grid + meter tick numbers along X and Y axes
inline void drawCartesianGrid(int halfExtentUnits = 20, float step = 1.0f) {
    float limit = static_cast<float>(halfExtentUnits) * step;
    for (int i = -halfExtentUnits; i <= halfExtentUnits; ++i) {
        float pos = static_cast<float>(i) * step;
        drawLine(pos, -limit, pos, limit, kGridLine, 1.0f);
        drawLine(-limit, pos, limit, pos, kGridLine, 1.0f);
    }
    drawLine(-limit, 0.0f, limit, 0.0f, kAxisX, 2.0f);
    drawLine(0.0f, -limit, 0.0f, limit, kAxisY, 2.0f);

    // Draw meter scale markers every 2 meters along X and Y axes
    for (int i = -12; i <= 12; i += 2) {
        if (i == 0) continue;
        drawTextWorld(static_cast<float>(i) - 0.3f, -0.45f, kGray, "%dm", i);
    }
    for (int j = -6; j <= 6; j += 2) {
        if (j == 0) continue;
        drawTextWorld(0.15f, static_cast<float>(j) - 0.15f, kGray, "%dm", j);
    }
}

struct SimpleWindow {
    SDL_Window* window{nullptr};
    SDL_GLContext glContext{nullptr};
    bool running{true};

    SimpleWindow(const char* title, int width = 1280, int height = 720) {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            std::cerr << "[Error] SDL_Init failed: " << SDL_GetError() << "\n";
            running = false;
            return;
        }
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        window = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        glContext = SDL_GL_CreateContext(window);
        SDL_GL_SetSwapInterval(1);
    }

    ~SimpleWindow() {
        if (glContext) SDL_GL_DestroyContext(glContext);
        if (window) SDL_DestroyWindow(window);
        SDL_Quit();
    }

    bool pollEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = false;
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) running = false;
        }
        return running;
    }

    bool isKeyHeld(SDL_Scancode key) const {
        const bool* keys = SDL_GetKeyboardState(nullptr);
        return keys[key];
    }

    void beginFrame(float pixelsPerMeter = 50.0f) {
        int w = 1280, h = 720;
        SDL_GetWindowSizeInPixels(window, &w, &h);
        clearScreen();
        setup2DCartesianView(w, h, pixelsPerMeter);
        drawCartesianGrid(20, 1.0f);
    }

    void endFrame() { SDL_GL_SwapWindow(window); }
};

}  // namespace gl_draw

#endif  // GAMEPHYSICS_GLDRAW_HPP
