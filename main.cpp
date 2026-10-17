// Single translation unit build: main.cpp includes every source file,
// so the program is built with one compiler command (see Makefile).
#include "game.h"
#include "game.cpp"
#include "texture_manager.cpp"
#include "map.cpp"
#include "vector_2d.cpp"
#include "ecs/collision.cpp"
#include "ecs/ecs.cpp"

#include <cstring>

// Control the max frame rate.
const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;

// Game declaration.
Game *game = nullptr;

int main(int argc, char *argv[]) {
    // Start in fullscreen mode with the "--fullscreen" option.
    bool full_screen = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--fullscreen") == 0) full_screen = true;
    }

    // Generate a new game.
    game = new Game();
    uint32_t frame_start = 0;
    int frame_time = 0;

    // Initialize the game.
    game->initialize(
        "暈到咪咪冒冒小狗的下船大作戰！",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
        full_screen
    );
    while (game->running()) {
        // Obtain the start time of this game loop.
        frame_start = SDL_GetTicks();

        // Refresh the game in the order below:
        // 1. Handle the events that occur in the game process.
        // 2. Update the data of the game elements.
        // 3. Refresh the screen so that it shows the current status of the game elements.
        game->handle_events();
        game->update();
        game->render();

        // Cap the max frame rate.
        frame_time = SDL_GetTicks() - frame_start;
        if (FRAME_DELAY > frame_time) {
            SDL_Delay(FRAME_DELAY - frame_time);
        }
    }

    // When the game loop is done, clean the game.
    game->clean();
    return 0;
}
