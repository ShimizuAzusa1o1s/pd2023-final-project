#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>

class ColliderComponent;

class Game {
private:
    bool is_running; // Indicator of the game-running status.
    SDL_Window *window;

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
};
