#pragma once
#include <SDL2/SDL.h>
#include "components.h"
#include "../textureManager.h"

class SpriteComponent : public Component
{
    private:
        TransformComponent* transform;
        SDL_Texture* texture;
        SDL_Rect srcRect, destRect;
    public:
        //Constructors.
        SpriteComponent() = default;
        SpriteComponent( const char* path ){
            setTexture( path );
        }
        //Destructors.
        ~SpriteComponent(){
            SDL_DestroyTexture( texture );
        }
        //Set the component's image.
        void setTexture( const char* path ){
            this->texture = TextureManager::loadTexture( path );
        }
        //Initialize the component
        void init() override{
            this->transform = &entity->getComponent< TransformComponent >();
            this->srcRect.x = this->srcRect.y = 0;
            this->srcRect.w = transform->width;
            this->srcRect.h = transform->height;
        }
        //Update the render position.
        void update() override{
            this->destRect.x = static_cast<int>(this->transform->position.x);
            this->destRect.y = static_cast<int>(this->transform->position.y);
            this->destRect.w = transform->width * transform->scale;
            this->destRect.h = transform->height * transform->scale;
        }
        //Draw the texture.
        void draw() override{
            TextureManager::drawTexture( this->texture, this->srcRect, this->destRect );
        }
};