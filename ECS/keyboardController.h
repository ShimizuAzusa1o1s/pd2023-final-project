#pragma once
#include "../game.h"
#include "ECS.h"
#include "components.h"

class KeyboardController : public Component
{
    private:
        const Uint8* keyStates = SDL_GetKeyboardState(NULL);
    public:
        TransformComponent *transform;

        void init() override{
            transform = &entity->getComponent< TransformComponent >();
        }

        void update() override{
            //If W-A-S-D is pressed, altering velocity.
            if( Game::event.type == SDL_KEYDOWN ){
                switch( Game::event.key.keysym.scancode ){
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
            //If W-A-S-D is released, altering velocity.
            //In this system, the key that is preesed latter will be recorded.
            if( Game::event.type == SDL_KEYUP ){
                switch( Game::event.key.keysym.scancode ){
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_S:
                        transform->velocity.y = keyStates[SDL_SCANCODE_S] - keyStates[SDL_SCANCODE_W];
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_D:
                        transform->velocity.x = keyStates[SDL_SCANCODE_D] - keyStates[SDL_SCANCODE_A];
                        break;
                    default:;
                }
            }
        }
};