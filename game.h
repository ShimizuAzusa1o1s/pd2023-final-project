#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>

class ColliderComponent;

// Logical size of the game screen in pixels. The field is 800 * 640,
// and the right 100 pixels show the health points.
// SDL scales this size to the window, also in fullscreen mode.
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 640;

class Game {
private:
    bool is_running; // Indicator of the game-running status.
    SDL_Window *window;
    SDL_Texture *frame = nullptr; // One frame at the logical size, scaled to the window.

public:
    // Static member variables and functions.
    static void add_tile(int id, int x, int y);
    static SDL_Renderer *renderer;
    static SDL_Event event;
    static std::vector<ColliderComponent *> colliders;

    // Constructor.
    Game();
    // Destructor.
    ~Game() {}

    // Member functions.
    void initialize(const char *title, int x_pos, int y_pos, int width, int height, bool full_screen);
    void handle_events();
    void update();
    void render();
    void clean();
    bool running();
    void toggle_full_screen();
};
