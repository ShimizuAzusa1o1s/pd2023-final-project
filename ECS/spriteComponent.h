#pragma once
#include <SDL2/SDL.h>
#include "component.h"

class SpriteComponent : public Component
{
    private:
        PositionComponent* position;
        SDL_Texture* texture;
        SDL_Rect srcRect, destRect;
    public:
        //Constructors.
        SpriteComponent() = default;
        SpriteComponent( const char* path ){
            setTexture( path );
        }

        void setTexture( const char* path ){
            this->texture = TextureManager::loadTexture( path );
        }

        void init() override{
            this->position = &entity->getComponent< PositionComponent >();
            this->srcRect.x = this->srcRect.y = 0;
            this->srcRect.w = this->srcRect.h = 32;
            this->destRect.w = this->destRect.h = 64;
        }

        void update() override{
            this->destRect.x = this->position->x();
            this->destRect.y = this->position->y();
        }

        void draw() override{
            TextureManager::draw( this->texture, this->srcRect, this->destRect );
        }
};