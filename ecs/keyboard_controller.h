#pragma once
#include "../game.h"
#include "ecs.h"
#include "components.h"

class KeyboardController : public Component {
private:
    const Uint8 *key_states = SDL_GetKeyboardState(NULL);

public:
    TransformComponent *transform;

    void init() override {
        transform = &entity->get_component<TransformComponent>();
    }

    // Game::handle_events() calls this function once for each event.
    void handle_event(const SDL_Event &event) {
        // If W-A-S-D is pressed, alter the velocity.
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                    transform->velocity.y = -1;
                    break;
                case SDL_SCANCODE_A:
                    transform->velocity.x = -1;
                    break;
                case SDL_SCANCODE_D:
                    transform->velocity.x = 1;
                    break;
                case SDL_SCANCODE_S:
                    transform->velocity.y = 1;
                    break;
                default:;
            }
        }
        // If W-A-S-D is released, alter the velocity.
        // In this system, the key that is pressed later will be recorded.
        if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_S:
                    transform->velocity.y = key_states[SDL_SCANCODE_S] - key_states[SDL_SCANCODE_W];
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_D:
                    transform->velocity.x = key_states[SDL_SCANCODE_D] - key_states[SDL_SCANCODE_A];
                    break;
                default:;
            }
        }
    }
};
